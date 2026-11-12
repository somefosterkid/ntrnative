#!/usr/bin/env python3
# tools/evalorder_scan.py: scan the game C the port compiles for evaluation-order traps between Metrowerks (the DS) and clang
# (every PC target). The one shape that bites (2026-11-04, the N-battle hang, the maintainers' notes "Lessons"): a left operand
# read beside a call that moves the same field, `vm->pc + (s32)VM_Read32(vm)`: MWCC loads vm->pc after the call, clang
# before. Compound `x += f(&x)` and call arguments evaluate alike on both (checked against the ROM). Prints candidates
# (file:line, kind); review each by hand. Run with no arguments from anywhere: it reads tools/port_sources.py's list and maps
# port/extern wrappers back to their src/ files. Regex-based, callees followed two levels, function pointers and macros not
# resolved.
# Evaluation-order audit of the game C the port compiles.
#  A: in one unsequenced segment, a read of X's field (or *X / X[..]) beside a call that gets X, where the callee (to 2
#     levels) writes a field of that name;
#  B: two calls in one segment that both get X and both mutate X (any member write / *p write / ++), e.g. f(Read(vm), Read(vm)).
import re, subprocess, sys, glob
import os
root=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# --b2w2 DIR: Black 2's and White 2's C instead (pokebw2's checkout, tools/pokebw2_fetch.sh; the maintainers' notes)
b2=sys.argv[sys.argv.index('--b2w2')+1] if '--b2w2' in sys.argv else None
files=subprocess.run(['python3',root+'/tools/port_sources.py']+(['--b2w2',b2] if b2 else []),capture_output=True,text=True).stdout.split()
files=[f.replace('/port/extern/','/') for f in files if '/port/src/' not in f]
if b2:
    files=[f.replace(root+'/port/extern_b2w2/',os.path.abspath(b2)+'/') for f in files]
    root=os.path.abspath(b2)
hdrs=glob.glob(root+'/include/**/*.h',recursive=True)+glob.glob(root+'/lib/*/include/**/*.h',recursive=True)+glob.glob(root+'/port/include/**/*.h',recursive=True)
ident=r'[A-Za-z_]\w*'
callre=re.compile(r'\b('+ident+r')\s*\(')
defre=re.compile(r'^(?:static\s+)?(?:inline\s+)?(?:static\s+)?[\w\s\*]+?\b(\w+)\s*\(([^;{)]*)\)\s*\{',re.M)
def clean(txt):
    txt=re.sub(r'//.*','',txt); txt=re.sub(r'/\*.*?\*/','',txt,flags=re.S); return txt
defs={}
for f in files+hdrs:
    try: txt=clean(open(f,errors='replace').read())
    except: continue
    for m in defre.finditer(txt):
        i=m.end(); d=1
        while i<len(txt) and d:
            if txt[i]=='{': d+=1
            elif txt[i]=='}': d-=1
            i+=1
        defs.setdefault(m.group(1),(m.group(2),txt[m.end():i]))
def pname(params,idx):
    ps=[p.strip() for p in params.split(',')]
    if idx>=len(ps): return None
    pm=re.search(r'(\w+)\s*(?:\[\w*\])?\s*$',ps[idx]); return pm.group(1) if pm else None
WR=r'(?:[-+*/|&^]?=(?!=)|\+\+|--)'
def writes_field(fn,fields,depth,seen):
    if fn in seen or fn not in defs or depth>2: return None
    seen.add(fn); body=defs[fn][1]
    for fl in fields:
        if re.search(r'(?:->|\.)\s*'+fl+r'\b(?:\s*\[[^\]]*\])?\s*'+WR,body) or re.search(r'(?:\+\+|--)\s*[\w\->\.]*(?:->|\.)'+fl+r'\b',body):
            return fn+'.'+fl
    for c in set(re.findall(r'\b(\w+)\s*\(',body)):
        r=writes_field(c,fields,depth+1,seen)
        if r: return r
    return None
def mutates_param(fn,idx,depth,seen):
    """does fn write through its idx-th parameter (directly, to 2 levels)?"""
    if (fn,idx) in seen or fn not in defs or depth>2: return False
    seen.add((fn,idx)); params,body=defs[fn]; p=pname(params,idx)
    if not p: return False
    if re.search(r'(?<![\w.])'+p+r'\s*(?:->|\.)\s*[\w\.\->]*(?:\[[^\]]*\])?\s*'+WR,body): return True
    if re.search(r'\*\s*'+p+r'\s*'+WR,body) or re.search(r'(?<![\w.])'+p+r'\s*\[[^\]]*\]\s*'+WR,body): return True
    if re.search(r'(?:\+\+|--)\s*\*?\s*'+p+r'\b|(?<![\w.])'+p+r'\s*(?:\+\+|--)',body) and '*' in params.split(',')[idx]: return True
    # passed on: which argument index
    for cm in callre.finditer(body):
        a,e=args_of(body,cm.end()-1)
        if a is None: continue
        for i,arg in enumerate(split_args(a)):
            if re.search(r'(?<![\w.>])&?'+p+r'\b(?!\s*[\(\[])',arg) and not re.search(r'(?<![\w.>])'+p+r'\s*(?:->|\.)',arg):
                if mutates_param(cm.group(1),i,depth+1,seen): return True
    return False
def args_of(s,i):
    d=0
    for j in range(i,len(s)):
        if s[j]=='(': d+=1
        elif s[j]==')':
            d-=1
            if d==0: return s[i+1:j], j
    return None,None
def split_args(a):
    out=[]; d=0; cur=''
    for ch in a:
        if ch in '([': d+=1
        elif ch in ')]': d-=1
        if ch==',' and d==0: out.append(cur); cur=''; continue
        cur+=ch
    out.append(cur); return out
kw={'if','while','for','switch','return','sizeof','defined','NELEMS'}
seqsplit=re.compile(r'&&|\|\||\?|(?<!:):(?!:)|;')
def strip(s):
    s=re.sub(r'"(\\.|[^"\\])*"','""',s); s=re.sub(r"'(\\.|[^'\\])'","'c'",s); return s
for f in files:
    try: txt=clean(open(f,errors='replace').read())
    except: continue
    lines=txt.split('\n'); buf=''; start=0
    for n,line in enumerate(lines,1):
        if not buf: start=n
        buf+=' '+line.strip()
        if not buf.rstrip().endswith((';','{','}',':')) and len(buf)<600: continue
        st=strip(buf); buf=''
        if st.lstrip().startswith('#'): continue
        loc=f.replace(root+'/','')+':'+str(start)
        # segments
        bounds=[0]+[x.end() for x in seqsplit.finditer(st)]+[len(st)]
        for si in range(len(bounds)-1):
            lo,hi=bounds[si],bounds[si+1]
            seg=st[lo:hi]
            calls=[]
            for m in callre.finditer(seg):
                if m.group(1) in kw: continue
                a,e=args_of(seg,m.end()-1)
                if a is None: continue
                calls.append((m.group(1),m.start(),e,a))
            # B: two mutating calls on X
            mut={}  # x -> [callnames]
            for name,s0,e0,a in calls:
                for i,arg in enumerate(split_args(a)):
                    for x in set(re.findall(r'(?<![\w.>])&?('+ident+r')\b(?!\s*[\(\[]|\s*->|\s*\.)',arg)):
                        if x in kw or x.isupper() or x in ('NULL','TRUE','FALSE'): continue
                        if name not in defs:
                            if re.search(r'Read|Pop|Push|Next|Alloc',name): mut.setdefault(x,[]).append(name+'?')
                            continue
                        if mutates_param(name,i,0,set()): mut.setdefault(x,[]).append(name)
            for x,names in mut.items():
                if len(names)>=2: print('B',loc,'['+x+' via '+'+'.join(names)+']',st.strip()[:200])
            # A: field read beside a call that writes that field
            for name,s0,e0,a in calls:
                outside=seg[:s0]+' '+seg[e0+1:]
                for x in set(re.findall(r'(?<![\w.>])&?('+ident+r')\b(?!\s*[\(\[]|\s*->|\s*\.)',a)):
                    if x in kw or x.isupper() or x in ('NULL','TRUE','FALSE'): continue
                    fields=set(re.findall(r'(?<![\w.])'+x+r'\s*(?:->|\.)\s*(\w+)',outside))
                    if not fields: continue
                    r=writes_field(name,fields,0,set())
                    if r: print('A',loc,'['+x+' via '+name+' writes '+r+']',st.strip()[:200])
