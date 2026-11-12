// Ghidra's decompilation of every function of Pokémon Black's ARM9 modules, for tools/ghidra_draft.py, which writes
// the manifest and runs this headless:
//
//     analyzeHeadless <project dir> <name> -deleteProject -scriptPath tools/ghidra
//         -preScript BlackDrafts.java <manifest> <output dir> <threads>
//
// The manifest's lines, tab-separated: "context <C file>", "module <name> <binary> <symbols.txt> <section>..." with
// each section "<name>:<start>:<end>:<kind>", "prototype <function> <return type> <"..." or ""> <type>|<name>...",
// "callee" lines like it, "retype <function> <register> <type>" (a struct pointer tools/struct_infer.py's pointer
// flow proves a parameter of a function without a "prototype" is), "passes <function> <least> <most>" (how many of
// r0-r3 its calls pass), "register <name> <address> <bits>", "global <datum> <type>" and "program <module> <other
// modules>". The context (the headers, preprocessed) is parsed once by Ghidra's C parser, for the structs' fields and
// the typedefs and enums that the prototypes and globals name. Each program is built in memory, never saved: the module's sections at their addresses
// (.text read-only and executable, so literal pools fold into constants; .bss uninitialized), the other modules'
// sections uninitialized where they don't overlap, which gives calls and globals their names, and the I/O registers.
// Black's functions get their names, sizes and ARM/Thumb modes, and the prototypes, else the "callee" ones (the last
// drafts'). The data get their names and the globals' types. No auto-analysis: the decompiler follows the code itself.
// The module's functions without a "prototype" are decompiled once, one at a time, each with its own unlocked and its
// callees' as they are, to give them the parameters and return their code uses (and those its calls pass, and a linker
// veneer what its function takes), a parameter a "retype" line names its struct pointer; then each function of the
// module is decompiled to
// <output dir>/<module>/<function>.c, or a "// Ghidra failed" line, followed by a "/* global <name> <address> <size>
// <its variable's size> <type> | <its variable's type> */" line for each global the C uses.
// @category bw-port

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.app.util.cparser.C.CParser;
import ghidra.program.database.ProgramDB;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.data.ArrayDataType;
import ghidra.program.model.data.ByteDataType;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.DataUtilities;
import ghidra.program.model.data.DoubleDataType;
import ghidra.program.model.data.FloatDataType;
import ghidra.program.model.data.IntegerDataType;
import ghidra.program.model.data.LongLongDataType;
import ghidra.program.model.data.AbstractIntegerDataType;
import ghidra.program.model.data.Pointer;
import ghidra.program.model.data.PointerDataType;
import ghidra.program.model.data.ShortDataType;
import ghidra.program.model.data.SignedByteDataType;
import ghidra.program.model.data.StandAloneDataTypeManager;
import ghidra.program.model.data.StructureDataType;
import ghidra.program.model.data.TypeDef;
import ghidra.program.model.data.TypedefDataType;
import ghidra.program.model.data.Undefined;
import ghidra.program.model.data.UnsignedIntegerDataType;
import ghidra.program.model.data.UnsignedLongLongDataType;
import ghidra.program.model.data.UnsignedShortDataType;
import ghidra.program.model.data.VoidDataType;
import ghidra.program.model.lang.Language;
import ghidra.program.model.lang.LanguageID;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.FlowOverride;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Function.FunctionUpdateType;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Parameter;
import ghidra.program.model.listing.ParameterImpl;
import ghidra.program.model.listing.ProgramContext;
import ghidra.program.model.listing.ReturnParameterImpl;
import ghidra.program.model.listing.Variable;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.pcode.HighFunction;
import ghidra.program.model.pcode.HighFunctionDBUtil;
import ghidra.program.model.pcode.HighSymbol;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.util.DefaultLanguageService;
import ghidra.util.task.TaskMonitor;

public class BlackDrafts extends GhidraScript {
	// name kind:function(thumb,size=0xf8) addr:0x021f36c0
	static final Pattern SYMBOL = Pattern.compile("^(\\S+) kind:(\\w+)(?:\\((.*)\\))? addr:0x([0-9a-fA-F]+)");
	static final Pattern SIZE = Pattern.compile("size=0x([0-9a-fA-F]+)");

	record Section(String name, long start, long end, String kind) {}

	record Module(String name, String binary, String symbols, List<Section> sections) {}

	record Symbol(String name, String kind, boolean thumb, long size, long addr) {}

	final Map<String, List<Symbol>> symbols = new HashMap<>();
	final Map<String, String[]> prototypes = new HashMap<>();
	final Map<String, String[]> callees = new HashMap<>(); // the last drafts' prototypes, which callers see
	final List<String[]> registers = new ArrayList<>(); // "register <name> <address> <bits>", by address
	final Map<String, DataType> types = new HashMap<>(); // the program's, made again for each
	final Map<String, DataType> parsed = new HashMap<>(); // the context's types, by name
	final Map<String, String> globals = new HashMap<>(); // "global <datum> <type>"
	final Map<String, int[]> passes = new HashMap<>(); // "passes <function> <least> <most>", r0-r3 its calls pass
	final Map<String, List<String[]>> retypes = new HashMap<>(); // "retype <function> <register> <type>"
	ProgramDB program;

	@Override
	public void run() throws Exception {
		String[] args = getScriptArgs();
		Path out = Paths.get(args[1]);
		int threads = Integer.parseInt(args[2]);
		Map<String, Module> modules = new HashMap<>();
		List<String[]> programs = new ArrayList<>();
		String context = null;
		for (String line : Files.readAllLines(Paths.get(args[0]), StandardCharsets.UTF_8)) {
			String[] f = line.split("\t", -1);
			if (f[0].equals("module")) {
				List<Section> sections = new ArrayList<>();
				for (int i = 4; i < f.length; i++) {
					String[] s = f[i].split(":");
					sections.add(new Section(s[0], Long.decode(s[1]), Long.decode(s[2]), s[3]));
				}
				modules.put(f[1], new Module(f[1], f[2], f[3], sections));
			}
			else if (f[0].equals("program")) {
				programs.add(f);
			}
			else if (f[0].equals("prototype")) {
				prototypes.putIfAbsent(f[1], f);
			}
			else if (f[0].equals("callee")) {
				callees.putIfAbsent(f[1], f);
			}
			else if (f[0].equals("register")) {
				registers.add(f);
			}
			else if (f[0].equals("global")) {
				globals.put(f[1], f[2]);
			}
			else if (f[0].equals("retype")) {
				retypes.computeIfAbsent(f[1], k -> new ArrayList<>()).add(f);
			}
			else if (f[0].equals("passes")) {
				passes.put(f[1], new int[] { Integer.parseInt(f[2]), Integer.parseInt(f[3]) });
			}
			else if (f[0].equals("context")) {
				context = f[1];
			}
		}
		Language lang = DefaultLanguageService.getLanguageService()
				.getLanguage(new LanguageID("ARM:LE:32:v5t"));
		if (context != null) {
			parse(context, lang);
		}
		int total = 0, failed = 0;
		long t0 = System.currentTimeMillis();
		for (String[] p : programs) {
			if (monitor.isCancelled()) {
				break;
			}
			long t = System.currentTimeMillis();
			Module own = modules.get(p[1]);
			program = new ProgramDB(own.name, lang, lang.getDefaultCompilerSpec(), this);
			types.clear(); // a type belongs to its program
			try {
				List<Function> functions = load(program, own, p, modules);
				// first the functions without a "prototype", each unlocked, whose parameters and return are then the
				// ones Ghidra saw them use, so that the calls in the module pass what the callee takes; then all. A
				// linker veneer takes what the function it branches to takes, before and after
				Map<Function, Function> veneers = veneers(program, functions);
				List<Function> free = functions.stream().filter(f -> !prototypes.containsKey(f.getName())
						&& !(veneers.containsKey(f) && veneers.get(f).getSignatureSource() != SourceType.DEFAULT))
						.toList();
				signatures(program, veneers);
				unlocked(program, free);
				retype(program, free);
				signatures(program, veneers);
				int[] result = decompile(program, functions, out.resolve(own.name), threads);
				total += result[0];
				failed += result[1];
				println(String.format("%s: %d functions, %d failed, %.1f s", own.name, result[0], result[1],
					(System.currentTimeMillis() - t) / 1000.0));
			}
			catch (Exception e) {
				printerr(own.name + ": " + e);
			}
			finally {
				program.release(this);
			}
		}
		println(String.format("all: %d functions, %d failed, %.0f s", total, failed,
			(System.currentTimeMillis() - t0) / 1000.0));
	}

	List<Function> load(ProgramDB program, Module own, String[] p, Map<String, Module> modules)
			throws Exception {
		List<Function> functions = new ArrayList<>();
		int tx = program.startTransaction("load");
		try {
			Memory mem = program.getMemory();
			AddressSpace space = program.getAddressFactory().getDefaultAddressSpace();
			byte[] bytes = Files.readAllBytes(Paths.get(own.binary));
			long base = own.sections.stream().mapToLong(Section::start).min().getAsLong();
			for (Section s : own.sections) {
				if (s.end <= s.start) {
					continue;
				}
				Address a = space.getAddress(s.start);
				MemoryBlock b = s.kind.equals("bss")
						? mem.createUninitializedBlock(own.name + s.name, a, s.end - s.start, false)
						: mem.createInitializedBlock(own.name + s.name, a,
							new ByteArrayInputStream(bytes, (int) (s.start - base), (int) (s.end - s.start)),
							s.end - s.start, monitor, false);
				b.setPermissions(true, !s.kind.equals("code"), s.kind.equals("code"));
			}
			List<Module> others = new ArrayList<>();
			for (int i = 2; i < p.length; i++) {
				Module m = modules.get(p[i]);
				others.add(m);
				for (Section s : m.sections) {
					if (s.end <= s.start) {
						continue;
					}
					try {
						MemoryBlock b = mem.createUninitializedBlock(m.name + s.name, space.getAddress(s.start),
							s.end - s.start, false);
						b.setPermissions(true, true, s.kind.equals("code"));
					}
					catch (Exception e) {
						// overlaps a block already there: this module's own, or a module loaded in its place
					}
				}
			}
			// the I/O registers by their names and widths, in a block for each run of them
			long ioStart = -1, ioEnd = -1;
			for (int i = 0; i <= registers.size(); i++) {
				long a = i < registers.size() ? Long.decode(registers.get(i)[2]) : Long.MAX_VALUE;
				if (ioStart >= 0 && a - ioEnd > 0x10000) {
					mem.createUninitializedBlock("io_" + Long.toHexString(ioStart), space.getAddress(ioStart),
						ioEnd - ioStart, false).setPermissions(true, true, false);
					ioStart = -1;
				}
				if (i < registers.size()) {
					ioStart = ioStart < 0 ? a : ioStart;
					ioEnd = Math.max(ioEnd, a + Integer.parseInt(registers.get(i)[3]) / 8);
				}
			}
			for (String[] r : registers) {
				Address a = space.getAddress(Long.decode(r[2]));
				program.getSymbolTable().createLabel(a, r[1], SourceType.USER_DEFINED);
				try {
					DataUtilities.createData(program, a, type("u" + r[3]), -1,
						DataUtilities.ClearDataMode.CLEAR_ALL_CONFLICT_DATA);
				}
				catch (Exception e) {
					// overlaps a wider register's
				}
			}
			ProgramContext context = program.getProgramContext();
			Register tmode = context.getRegister("TMode");
			others.add(0, own);
			for (Module m : others) {
				for (Symbol s : symbols(m)) {
					Address a = space.getAddress(s.addr);
					MemoryBlock b = mem.getBlock(a);
					if (b == null || !b.getName().startsWith(m.name + ".")) {
						continue; // a module whose section overlapped another's
					}
					if (s.kind.equals("function") && s.size > 0) {
						Address end = a.add(s.size - 1);
						if (s.thumb) {
							context.setValue(tmode, a, end, BigInteger.ONE);
						}
						try {
							Function f = program.getFunctionManager()
									.createFunction(s.name, a, new AddressSet(a, end), SourceType.USER_DEFINED);
							if (m == own) {
								functions.add(f);
							}
							if (prototypes.containsKey(s.name)) {
								prototype(program, f, prototypes.get(s.name));
							}
							else if (callees.containsKey(s.name)) {
								prototype(program, f, callees.get(s.name));
							}
						}
						catch (Exception e) {
							printerr(own.name + ": no function " + s.name + ": " + e);
							program.getSymbolTable().createLabel(a, s.name, SourceType.USER_DEFINED);
						}
					}
					else if (s.kind.equals("data") || s.kind.equals("bss")) {
						program.getSymbolTable().createLabel(a, s.name, SourceType.USER_DEFINED);
						if (globals.containsKey(s.name)) {
							try {
								DataUtilities.createData(program, a, type(globals.get(s.name)), -1,
									DataUtilities.ClearDataMode.CLEAR_ALL_CONFLICT_DATA);
							}
							catch (Exception e) {
								// runs past its block, or over another datum
							}
						}
					}
				}
			}
			// MWCC's tail call, "ldr ip, =f; bx ip" ("bx r3" in Thumb): the decompiler followed the branch into the
			// function it reaches as if it were the caller's own code (func_ov016_021cf644 drafted
			// func_ov016_021cfe18's body), so a bx to anything but lr is a call that returns. Only that instruction
			// is disassembled: a function disassembled whole drafts its pool's words as DAT_ and loses arguments
			for (Function f : functions) {
				boolean thumb = context.getValue(tmode, f.getEntryPoint(), false) != null
						&& context.getValue(tmode, f.getEntryPoint(), false).signum() != 0;
				Address a = f.getEntryPoint(), end = f.getBody().getMaxAddress();
				for (; a.compareTo(end) < 0; a = a.add(thumb ? 2 : 4)) {
					long w = thumb ? mem.getShort(a) & 0xffffL : mem.getInt(a) & 0xffffffffL;
					long reg = thumb ? (w >> 3) & 0xf : w & 0xf;
					boolean bx = thumb ? (w & 0xff87) == 0x4700 : (w & 0xfffffff0L) == 0xe12fff10L;
					if (!bx || reg >= 14) {
						continue;
					}
					new DisassembleCommand(a, new AddressSet(a, a.add(thumb ? 1 : 3)), false).applyTo(program,
						TaskMonitor.DUMMY);
					Instruction ins = program.getListing().getInstructionAt(a);
					if (ins != null) {
						ins.setFlowOverride(FlowOverride.CALL_RETURN);
					}
				}
			}
		}
		finally {
			program.endTransaction(tx, true);
		}
		return functions;
	}

	// The module's linker veneers, each with the function it branches to (the link makes them, tools/veneers.py): a
	// Thumb "bx pc; nop" with the ARM "ldr ip, [pc]; bx ip; .word f" after it, that ARM part alone, or an ARM
	// "ldr pc, [pc, #-4]; .word f". Ghidra can't follow one, so a call to it passed what the caller's registers held.
	// Those symbols.txt no longer names (veneers.py took them out of a converted file's config) after a function
	// become thunks of their function, which the calls then name (func_0x021ce7c4 was one)
	Map<Function, Function> veneers(ProgramDB program, List<Function> functions) {
		Map<Function, Function> out = new HashMap<>();
		Memory mem = program.getMemory();
		Register tmode = program.getProgramContext().getRegister("TMode");
		int tx = program.startTransaction("veneers");
		try {
			for (Function f : functions) {
				long[] v = veneer(mem, f.getEntryPoint());
				Function target = v == null ? null : program.getFunctionManager()
						.getFunctionAt(f.getEntryPoint().getNewAddress(v[0] & ~1L));
				if (target != null && target != f) {
					out.put(f, target);
				}
				try {
					Address a = f.getBody().getMaxAddress().add(1);
					a = a.add((4 - a.getOffset() % 4) % 4);
					for (; (v = veneer(mem, a)) != null && program.getFunctionManager().getFunctionAt(a) == null;
							a = a.add(v[1])) {
						target = program.getFunctionManager().getFunctionAt(a.getNewAddress(v[0] & ~1L));
						if (target == null) {
							break;
						}
						if (v[1] == 16) {
							program.getProgramContext().setValue(tmode, a, a.add(3), BigInteger.ONE);
						}
						program.getFunctionManager().createFunction(null, a, new AddressSet(a, a.add(v[1] - 1)),
							SourceType.DEFAULT).setThunkedFunction(target);
					}
				}
				catch (Exception e) {
					// over another function, or past the block
				}
			}
		}
		finally {
			program.endTransaction(tx, true);
		}
		return out;
	}

	// {the address a veneer at a branches to, its size}, or null if there is none
	static long[] veneer(Memory mem, Address a) {
		if (a.getOffset() % 4 != 0) {
			return null;
		}
		try {
			if ((mem.getShort(a) & 0xffff) == 0x4778 && (mem.getShort(a.add(2)) & 0xffff) == 0x46c0
					&& mem.getInt(a.add(4)) == 0xe59fc000 && mem.getInt(a.add(8)) == 0xe12fff1c) {
				return new long[] { mem.getInt(a.add(12)) & 0xffffffffL, 16 };
			}
			if (mem.getInt(a) == 0xe59fc000 && mem.getInt(a.add(4)) == 0xe12fff1c) {
				return new long[] { mem.getInt(a.add(8)) & 0xffffffffL, 12 };
			}
			if (mem.getInt(a) == 0xe51ff004) {
				return new long[] { mem.getInt(a.add(4)) & 0xffffffffL, 8 };
			}
		}
		catch (Exception e) {
			// runs past its block
		}
		return null;
	}

	// Each veneer's prototype made its function's, when that has one; but a veneer that returns something keeps it
	// when its function returns nothing (a guess: the veneer's own prototype can be that of the C that matches, an
	// MWCC tail call "ldr ip, =f; bx ip", which returns what f does)
	void signatures(ProgramDB program, Map<Function, Function> veneers) {
		int tx = program.startTransaction("veneers");
		try {
			for (Map.Entry<Function, Function> e : veneers.entrySet()) {
				Function v = e.getKey(), f = e.getValue();
				if (f.getSignatureSource() == SourceType.DEFAULT) {
					continue;
				}
				try {
					List<Variable> params = new ArrayList<>();
					for (Variable p : f.getParameters()) {
						params.add(new ParameterImpl(p.getName(), p.getDataType(), program));
					}
					DataType ret = VoidDataType.isVoidDataType(f.getReturnType()) ? v.getReturnType()
							: f.getReturnType();
					v.updateFunction(null, new ReturnParameterImpl(ret, program), params,
						FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true, SourceType.USER_DEFINED);
					v.setVarArgs(f.hasVarArgs());
				}
				catch (Exception x) {
					printerr("veneer " + v.getName() + ": " + x);
				}
			}
		}
		finally {
			program.endTransaction(tx, true);
		}
	}

	// A prototype from this repo's headers: "prototype <name> <return> <"..." or ""> <type>|<name>..."
	void prototype(ProgramDB program, Function f, String[] p) {
		try {
			List<Variable> params = new ArrayList<>();
			for (int i = 4; i < p.length; i++) {
				String[] t = p[i].split("\\|", -1);
				params.add(new ParameterImpl(t[1].isEmpty() ? null : t[1], type(t[0]), program));
			}
			f.updateFunction(null, new ReturnParameterImpl(type(p[2]), program), params,
				FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true, SourceType.USER_DEFINED);
			f.setVarArgs(p[3].equals("..."));
		}
		catch (Exception e) {
			printerr("prototype of " + f.getName() + ": " + e);
		}
	}

	// The context's types: its structs with their fields, typedefs and enums
	void parse(String context, Language lang) throws Exception {
		long t = System.currentTimeMillis();
		StandAloneDataTypeManager dtm = new StandAloneDataTypeManager("context",
			lang.getDefaultCompilerSpec().getDataOrganization());
		CParser parser = new CParser(dtm, true, null);
		int tx = dtm.startTransaction("parse");
		try (InputStream in = Files.newInputStream(Paths.get(context))) {
			parser.parse(in);
		}
		catch (Exception e) {
			printerr("context: " + e.toString().split("\n")[0]);
		}
		finally {
			dtm.endTransaction(tx, true);
		}
		parsed.putAll(parser.getComposites());
		parsed.putAll(parser.getEnums());
		parsed.putAll(parser.getTypes());
		println(String.format("context: %d structs and unions, %d enums, %d typedefs, %.1f s",
			parser.getComposites().size(), parser.getEnums().size(), parser.getTypes().size(),
			(System.currentTimeMillis() - t) / 1000.0));
	}

	DataType resolve(DataType t) {
		return program.getDataTypeManager().resolve(t, null);
	}

	// "u16", a typedef of one ("HeapID=u16") or a struct's name, then its *s or an array's "[n]": the context's type
	// of that name, else the typedef, else a struct that has no fields here
	DataType type(String spec) {
		DataType t = types.get(spec);
		if (t != null) {
			return t;
		}
		String name = spec.contains("=") ? spec.substring(0, spec.indexOf('=')) : spec;
		if (spec.endsWith("]")) {
			DataType e = type(spec.substring(0, spec.indexOf('[')));
			t = new ArrayDataType(e, Integer.parseInt(spec.substring(spec.indexOf('[') + 1, spec.length() - 1)),
				e.getLength());
		}
		else if (spec.endsWith("*")) {
			t = new PointerDataType(type(spec.substring(0, spec.length() - 1)));
		}
		else if (parsed.containsKey(name) && !parsed.get(name).isNotYetDefined()) {
			t = resolve(parsed.get(name));
		}
		else if (spec.contains("=")) {
			t = new TypedefDataType(spec.substring(0, spec.indexOf('=')), type(spec.substring(spec.indexOf('=') + 1)));
		}
		else {
			t = switch (spec) {
				case "void" -> VoidDataType.dataType;
				case "u8" -> new TypedefDataType("u8", ByteDataType.dataType);
				case "s8" -> new TypedefDataType("s8", SignedByteDataType.dataType);
				case "u16" -> new TypedefDataType("u16", UnsignedShortDataType.dataType);
				case "s16" -> new TypedefDataType("s16", ShortDataType.dataType);
				case "u32" -> new TypedefDataType("u32", UnsignedIntegerDataType.dataType);
				case "s32" -> new TypedefDataType("s32", IntegerDataType.dataType);
				case "u64" -> new TypedefDataType("u64", UnsignedLongLongDataType.dataType);
				case "s64" -> new TypedefDataType("s64", LongLongDataType.dataType);
				case "f32" -> new TypedefDataType("f32", FloatDataType.dataType);
				case "f64" -> new TypedefDataType("f64", DoubleDataType.dataType);
				case "BOOL" -> new TypedefDataType("BOOL", IntegerDataType.dataType);
				default -> new StructureDataType(spec, 0);
			};
		}
		types.put(spec, t);
		return t;
	}

	List<Symbol> symbols(Module m) throws Exception {
		List<Symbol> list = symbols.get(m.name);
		if (list == null) {
			list = new ArrayList<>();
			for (String line : Files.readAllLines(Paths.get(m.symbols), StandardCharsets.UTF_8)) {
				Matcher s = SYMBOL.matcher(line);
				if (!s.find() || s.group(1).startsWith(".")) {
					continue;
				}
				String mode = s.group(3) == null ? "" : s.group(3); // "kind:bss addr:..." has none
				Matcher size = SIZE.matcher(mode);
				list.add(new Symbol(s.group(1), s.group(2), mode.startsWith("thumb"),
					size.find() ? Long.parseLong(size.group(1), 16) : 0, Long.parseLong(s.group(4), 16)));
			}
			symbols.put(m.name, list);
		}
		return list;
	}

	// The functions without a "prototype", one at a time in address order, each with its own parameters unlocked
	// (the last drafts' locked them, so that its callers pass what it took then): Ghidra sees again what it uses, its
	// callees' as they are locked, and the parameters and the return it saw are committed. One at a time: a caller
	// decompiled on another thread meanwhile saw an unlocked callee it could pass nothing to, and left out the
	// parameters it passes on (a wrapper came out with none every other run)
	void unlocked(ProgramDB program, List<Function> functions) {
		DecompInterface decompiler = new DecompInterface();
		DecompileOptions options = new DecompileOptions();
		options.grabFromProgram(program);
		decompiler.setOptions(options);
		decompiler.openProgram(program);
		int tx = program.startTransaction("parameters");
		try {
			for (Function f : functions) {
				if (monitor.isCancelled()) {
					break;
				}
				SourceType was = f.getSignatureSource();
				f.setSignatureSource(SourceType.DEFAULT);
				DecompileResults r = decompiler.decompileFunction(f, 60, TaskMonitor.DUMMY);
				if (!r.decompileCompleted() || r.getDecompiledFunction() == null || !commit(r)) {
					f.setSignatureSource(was);
				}
			}
		}
		finally {
			program.endTransaction(tx, true);
			decompiler.dispose();
		}
	}

	// The struct pointers tools/struct_infer.py's pointer flow proves the parameters of the functions without a
	// "prototype" are ("retype" lines), in place of the number or untyped pointer the unlocked pass gave them
	void retype(ProgramDB program, List<Function> functions) {
		int tx = program.startTransaction("retype");
		try {
			for (Function f : functions) {
				for (String[] r : retypes.getOrDefault(f.getName(), List.of())) {
					for (Parameter p : f.getParameters()) {
						if (p.getRegister() == null || !p.getRegister().getName().equals(r[2])
								|| p.getLength() != 4 || !loose(p.getDataType())) {
							continue;
						}
						try {
							p.setDataType(type(r[3]), SourceType.ANALYSIS);
						}
						catch (Exception e) {
							printerr("retype " + f.getName() + " " + r[2] + ": " + e);
						}
					}
				}
			}
		}
		finally {
			program.endTransaction(tx, true);
		}
	}

	// A number, or a pointer to nothing in particular (void, a number, undefined)
	static boolean loose(DataType t) {
		if (t instanceof TypeDef d) {
			t = d.getBaseDataType();
		}
		if (t instanceof Pointer p) {
			DataType d = p.getDataType();
			if (d instanceof TypeDef e) {
				d = e.getBaseDataType();
			}
			return d == null || d instanceof VoidDataType || d instanceof Undefined || d instanceof AbstractIntegerDataType;
		}
		return t instanceof AbstractIntegerDataType || t instanceof Undefined;
	}

	// Decompiles the functions on threads, each one's C to dir
	int[] decompile(ProgramDB program, List<Function> functions, Path dir, int threads) throws Exception {
		Files.createDirectories(dir);
		ConcurrentLinkedQueue<Function> queue = new ConcurrentLinkedQueue<>(functions);
		AtomicInteger failed = new AtomicInteger();
		List<Thread> workers = new ArrayList<>();
		for (int i = 0; i < Math.min(threads, functions.size()); i++) {
			Thread w = new Thread(() -> {
				DecompInterface decompiler = new DecompInterface();
				DecompileOptions options = new DecompileOptions();
				options.grabFromProgram(program);
				decompiler.setOptions(options);
				decompiler.openProgram(program);
				try {
					for (Function f; (f = queue.poll()) != null && !monitor.isCancelled();) {
						DecompileResults r = decompiler.decompileFunction(f, 60, TaskMonitor.DUMMY);
						boolean ok = r.decompileCompleted() && r.getDecompiledFunction() != null;
						String c = ok ? r.getDecompiledFunction().getC() + globals(r)
								: "// Ghidra failed: " + r.getErrorMessage().trim().replace("\n", " ") + "\n";
						if (!ok) {
							failed.incrementAndGet();
						}
						Files.writeString(dir.resolve(f.getName() + ".c"), c, StandardCharsets.UTF_8);
					}
				}
				catch (Exception e) {
					printerr(program.getName() + ": " + e);
				}
				finally {
					decompiler.dispose();
				}
			});
			w.start();
			workers.add(w);
		}
		for (Thread w : workers) {
			w.join();
		}
		return new int[] { functions.size(), failed.get() };
	}

	// The globals the C uses, one comment line each, "/* global <name> <address> <type> */": the type Ghidra gave
	// each, and the address of those it names by their address and type (iRam021b3ca4, as UNK_021b3ca4: no datum
	// starts there, or one in .bss), so that the draft can name them by the datum they are in
	static String globals(DecompileResults r) {
		List<String> out = new ArrayList<>();
		HighFunction hf = r.getHighFunction();
		for (Iterator<HighSymbol> it = hf == null ? null : hf.getGlobalSymbolMap().getSymbols();
				it != null && it.hasNext();) {
			HighSymbol s = it.next();
			try {
				out.add(String.format("/* global %s 0x%s %d %d %s | %s */\n", s.getName(),
					s.getStorage().getMinAddress().toString(false), s.getSize(),
					s.getHighVariable() == null ? -1 : s.getHighVariable().getSize(), s.getDataType().getDisplayName(),
					s.getHighVariable() == null ? "" : s.getHighVariable().getDataType().getDisplayName()));
			}
			catch (Exception e) {
				// a symbol without storage
			}
		}
		out.sort(null);
		return String.join("", out);
	}

	static final Pattern INCOMPLETE = Pattern.compile("\\bin_(r[0-3]|stack_\\w+)\\b");

	// Commits the parameters and the return Ghidra saw a function use, without the trailing parameters nothing
	// reads; false if it didn't: Ghidra's guess reads argument registers or stack its parameters leave out (in_r0),
	// so the function keeps the last draft's
	boolean commit(DecompileResults r) {
		Function f = r.getFunction();
		if (INCOMPLETE.matcher(r.getDecompiledFunction().getC()).find()) {
			return false;
		}
		try {
			HighFunctionDBUtil.commitParamsToDatabase(r.getHighFunction(), true,
				HighFunctionDBUtil.ReturnCommitOption.COMMIT, SourceType.ANALYSIS);
			// MWCC pushes r3 with the registers it saves to keep the stack 8-aligned: Ghidra takes it for a parameter
			// that the function saves in a variable nothing reads, and r1 and r2 before it for parameters it never
			// reads; the trailing ones nothing reads go, but not those the calls pass ("passes": the least), and u32
			// ones after them up to those, which Ghidra leaves out when the function never reads them; and none past
			// the most its calls pass (Ghidra reads an uninitialized variable's register as a parameter)
			String c = r.getDecompiledFunction().getC();
			int[] passed = passes.getOrDefault(f.getName(), new int[] { 0, 4 });
			for (int n = f.getParameterCount(); n > 0 && registers(f, n - 1) >= passed[0]
					&& unread(c, f.getParameter(n - 1).getName()); n--) {
				f.removeParameter(n - 1);
			}
			// (not a 64-bit one, which the call before can return)
			boolean wide = registers(f, f.getParameterCount()) > f.getParameterCount();
			for (int n = f.getParameterCount(); n > 0 && passed[1] < 4 && !wide && n > passed[1]; n--) {
				f.removeParameter(n - 1);
			}
			if (registers(f, f.getParameterCount()) < passed[0] && !f.hasVarArgs()) {
				List<Variable> params = new ArrayList<>(List.of(f.getParameters()));
				while (registers(params) < passed[0]) {
					params.add(new ParameterImpl(null, type("u32"), program));
				}
				f.updateFunction(null, f.getReturn(), params, FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true,
					SourceType.ANALYSIS);
			}
		}
		catch (Exception e) {
			printerr("parameters of " + f.getName() + ": " + e);
			return false;
		}
		return true;
	}

	// The argument registers a function's first n parameters take (two for a 64-bit one)
	static int registers(Function f, int n) {
		return registers(List.of(f.getParameters()).subList(0, n));
	}

	static int registers(List<? extends Variable> params) {
		return params.stream().mapToInt(p -> p.getLength() > 4 ? 2 : 1).sum();
	}

	// Whether C text has a parameter only in its signature, or saved in a variable that nothing reads
	static boolean unread(String c, String param) {
		int uses = count(c, param);
		Matcher saved = Pattern.compile("^\\s*(\\w+) = " + param + ";$", Pattern.MULTILINE).matcher(c);
		return uses == 1 || uses == 2 && saved.find() && count(c, saved.group(1)) == 2;
	}

	static int count(String c, String name) {
		Matcher m = Pattern.compile("\\b" + Pattern.quote(name) + "\\b").matcher(c);
		int n = 0;
		while (m.find()) {
			n++;
		}
		return n;
	}
}
