// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/struct_decls.h, the lines in use so far.
#ifndef BWPORT_STRUCT_DECLS_H
#define BWPORT_STRUCT_DECLS_H

// Every struct type is declared here once, and defined in the header of the module that owns it, if its layout is
// known

typedef union BattleAction BattleAction;
typedef struct BattleLoseData BattleLoseData;
typedef struct BattleCondition BattleCondition;
typedef struct AdventureSave AdventureSave;
typedef struct AppTaskMenu AppTaskMenu;
typedef struct AppTaskMenuRes AppTaskMenuRes;
typedef struct AreaMan AreaMan;
typedef struct AreaNPCSave AreaNPCSave;
typedef struct ArcTool ArcTool;
typedef struct BagSave BagSave;
typedef struct BGSys BGSys;
typedef struct BGSysBG BGSysBG;
typedef struct BmpCursor BmpCursor;
typedef struct BmpMenu BmpMenu;
typedef struct BmpOamActor BmpOamActor;
typedef struct BmpOamSys BmpOamSys;
typedef struct BmpWin BmpWin;
typedef struct Box2AppWork Box2AppWork;
typedef struct Box2SysWork Box2SysWork;
typedef struct BoxPkm BoxPkm;
typedef struct BoxSaveAccessor BoxSaveAccessor;
typedef struct BPlistMove BPlistMove;
typedef struct BPlistPokemon BPlistPokemon;
typedef struct BtlFieldSituation BtlFieldSituation;
typedef struct DayCareSave DayCareSave;
typedef struct DayCare DayCare;
typedef struct BPlistWork BPlistWork;
typedef struct BrBallEffect BrBallEffect;
typedef struct BrBtn BrBtn;
typedef struct BrFade BrFade;
typedef struct BrGraphic BrGraphic;
typedef struct BrMsgWin BrMsgWin;
typedef struct BrNet BrNet;
typedef struct BrProcSys BrProcSys;
typedef struct BrRes BrRes;
typedef struct BrSeq BrSeq;
typedef struct BrSidebar BrSidebar;
typedef struct BSubwayTrainer BSubwayTrainer;
typedef struct BtlvField BtlvField;
typedef struct ButtonMan ButtonMan;
typedef struct CalcCapsule CalcCapsule;
typedef struct CalcHitResult CalcHitResult;
typedef struct CalcSphere CalcSphere;
typedef struct DreamWorldSave DreamWorldSave;
typedef struct EffectEncountState EffectEncountState;
typedef struct EncData EncData;
typedef struct EncEff EncEff;
typedef struct EncEffCell EncEffCell;
typedef struct EncEffGrid EncEffGrid;
typedef struct EncEffGridParam EncEffGridParam;
typedef struct BtlSetup BtlSetup;
typedef struct DiveEventData DiveEventData;
typedef struct CityState CityState;
typedef struct EncountSave EncountSave;
typedef struct EncountState EncountState;
typedef struct EncountSystem EncountSystem;
typedef struct EventActionCallWork EventActionCallWork;
typedef struct EventActorJumpWork EventActorJumpWork;
typedef struct EventMakeArgs EventMakeArgs;
typedef struct EventSaveWork EventSaveWork;
typedef struct EventDendouMachineData EventDendouMachineData;
typedef struct EventWork EventWork;
typedef struct Field Field;
typedef struct FieldAcmdTCB FieldAcmdTCB;
typedef struct FieldActor FieldActor;
typedef struct FieldAsyncProc FieldAsyncProc;
typedef struct FieldAsyncProcManager FieldAsyncProcManager;
typedef struct FieldAppCallInput FieldAppCallInput;
typedef struct FieldAppCallWork FieldAppCallWork;
typedef struct FieldCamera FieldCamera;
typedef struct FieldChunk FieldChunk;
typedef struct FieldChunkContext FieldChunkContext;
typedef struct FieldChunkPropHolder FieldChunkPropHolder;
typedef struct FieldDispControl FieldDispControl;
typedef struct FieldExpObjAnm FieldExpObjAnm;
typedef struct FieldExpObjSystem FieldExpObjSystem;
typedef struct FieldFog FieldFog;
typedef struct FieldG3DMapper FieldG3DMapper;
typedef struct FieldMenuWork FieldMenuWork;
typedef struct FieldPlayer FieldPlayer;
typedef struct FieldPropAreaBounds FieldPropAreaBounds;
typedef struct FieldPropHandle FieldPropHandle;
typedef struct FieldPropSourceInfo FieldPropSourceInfo;
typedef struct FieldPropSystem FieldPropSystem;
typedef struct FieldRailSystem FieldRailSystem;
typedef struct FieldSaveAnime FieldSaveAnime;
typedef struct FieldScriptEnv FieldScriptEnv;
typedef struct FieldStatus FieldStatus;
typedef struct FieldSoundEmitter FieldSoundEmitter;
typedef struct FieldSubscreen FieldSubscreen;
typedef struct FieldTaskManager FieldTaskManager;
typedef struct FieldWFBC FieldWFBC;
typedef struct FishingEventWork FishingEventWork;
typedef struct FlyEventWork FlyEventWork;
typedef struct GameCommSys GameCommSys;
typedef struct GameData GameData;
typedef struct GameEvent GameEvent;
typedef struct GameRecords GameRecords;
typedef struct GameSystem GameSystem;
typedef struct GFLBitmap GFLBitmap;
typedef struct GimmickState GimmickState;
typedef struct GSyncMessage GSyncMessage;
typedef struct Font Font;
typedef struct GameProc GameProc;
typedef struct GameProcFunctions GameProcFunctions;
typedef struct GameProcManager GameProcManager;
typedef struct HiddenEventArgs HiddenEventArgs;
typedef struct IntrudeComm IntrudeComm;
typedef struct ISS ISS;
typedef struct ISS3DSoundSys ISS3DSoundSys;
typedef struct ItemData ItemData;
typedef struct KeyCursor KeyCursor;
typedef struct MapTerrainBuf MapTerrainBuf;
typedef struct MapTerrainSamplerOutput MapTerrainSamplerOutput;
typedef struct MBUtilMsg MBUtilMsg;
typedef struct MMSys MMSys;
typedef struct MonolithWork MonolithWork;
typedef struct MsgData MsgData;
typedef struct MusicalPoke MusicalPoke;
typedef struct MusicalProgram MusicalProgram;
typedef struct MusicalSave MusicalSave;
typedef struct MusicalShot MusicalShot;
typedef struct MusicalStageParam MusicalStageParam;
typedef struct MusItemData MusItemData;
typedef struct MusShotPhoto MusShotPhoto;
typedef struct MysteryGift MysteryGift;
typedef struct NetHandle NetHandle;
typedef struct NoGridMapper NoGridMapper;
typedef struct Ov112Work Ov112Work;
typedef struct PaletteFade PaletteFade;
typedef struct PartyPkm PartyPkm;
typedef struct PartySelectList PartySelectList;
typedef struct PlaceName PlaceName;
typedef struct PlayerActionPossibilities PlayerActionPossibilities;
typedef struct PlayerInfo PlayerInfo;
typedef struct PlayerState PlayerState;
typedef struct PMSData PMSData;
typedef struct PleasureBoat PleasureBoat;
typedef struct PMSInputData PMSInputData;
typedef struct PMSInputView PMSInputView;
typedef struct PMSInputWork PMSInputWork;
typedef struct PMSIParam PMSIParam;
typedef struct PMSISearch PMSISearch;
typedef struct PMSIVWordWin PMSIVWordWin;
typedef struct PokeDexSave PokeDexSave;
typedef struct PokeParty PokeParty;
typedef struct PrepareResidentActorsWork PrepareResidentActorsWork;
typedef struct PrintStream PrintStream;
typedef struct PStatusWork PStatusWork;
typedef struct RailPosition RailPosition;
typedef struct RailUnit RailUnit;
typedef struct Regulation Regulation;
typedef struct ResearchData ResearchData;
typedef struct SaveBlockDef SaveBlockDef;
typedef struct SaveControl SaveControl;
typedef struct SaveControlIntr SaveControlIntr;
typedef struct SaveDataTable SaveDataTable;
typedef struct ScriptWork ScriptWork;
typedef struct StaActing StaActing;
typedef struct StaActScriptSys StaActScriptSys;
typedef struct StrBuf StrBuf;
typedef struct SweetScentEventData SweetScentEventData;
typedef struct SweetScentPalette SweetScentPalette;
typedef struct SweetScentScreenWork SweetScentScreenWork;
typedef struct TalkMsgWinSys TalkMsgWinSys;
typedef struct TCBEx TCBEx;
typedef struct TCBExManager TCBExManager;
typedef struct TCBManager TCBManager;
typedef struct TextPrintParam TextPrintParam;
typedef struct TrainerClashSlot TrainerClashSlot;
typedef struct TrainerGameInfoSave TrainerCardSave;
typedef struct TrainerGameInfoSave TrainerGameInfoSave;
typedef struct UnityTowerSurveySave UnityTowerSurveySave;
typedef struct VM VM;
typedef struct VRAMAlloc VRAMAlloc;
typedef struct VRAMMan VRAMMan;
typedef struct WaitIcon WaitIcon;
typedef struct TCB TCB;
typedef struct TrainerDataSave TrainerDataSave;
typedef struct UnderwaterEffectWork UnderwaterEffectWork;
typedef struct WifiBattleMatchLinkPlayer WifiBattleMatchLinkPlayer;
typedef struct WifiBattleMatchLinkRanking WifiBattleMatchLinkRanking;
typedef struct WifiBattleMatchLinkRatings WifiBattleMatchLinkRatings;
typedef struct WifiBattleMatchLinkReport WifiBattleMatchLinkReport;
typedef struct WifiList WifiList;
typedef struct WipeWnd WipeWnd;
typedef struct WorldTradeData WorldTradeData;
typedef struct WordSet WordSet;
typedef struct ZoneSpawnInfo ZoneSpawnInfo;

// NitroSystem's, by the tag NitroSystem gives it, which nnsys/g3d.h defines
typedef struct NNSG3dRenderObj_ NNSG3dRenderObj;

typedef struct BattleMonDamageRecord BattleMonDamageRecord;
typedef struct BtlServerClient BtlServerClient;
typedef struct BtlFlowMoveParam BtlFlowMoveParam;
typedef struct SwarmData SwarmData;
typedef struct EntreeForestSpawnContext EntreeForestSpawnContext;
typedef struct Demo3DParam Demo3DParam;
typedef struct ListMenuOption ListMenuOption;
typedef struct ListMenuUI ListMenuUI;
typedef struct GridPos GridPos;
typedef struct ActorPositionRail ActorPositionRail;
typedef struct FieldCameraAnimation FieldCameraAnimation;
typedef struct FieldG3DMapperTerrain FieldG3DMapperTerrain;
typedef struct MapReplaceVariables MapReplaceVariables;
typedef struct FieldPalaceSys FieldPalaceSys;
typedef struct FieldSceneAreaLoader FieldSceneAreaLoader;
typedef struct ScriptListMenu ScriptListMenu;
typedef struct FieldTerrainAnimatorSetup FieldTerrainAnimatorSetup;
typedef struct MailData MailData;
typedef struct GlyphInfo GlyphInfo;
typedef struct MCSSLoadInfo MCSSLoadInfo;
typedef struct AdventureTime AdventureTime;
typedef struct ActionOrderEntry ActionOrderEntry;
typedef struct AreaData AreaData;
typedef struct BmpMenuList BmpMenuList;
typedef struct Calendar Calendar;
typedef struct FldCommActSys FldCommActSys;
typedef struct GdsProfile GdsProfile;
typedef struct BGMInfo BGMInfo;
typedef struct BGMStackPreload BGMStackPreload;
typedef struct BattleBoxSave BattleBoxSave;
typedef struct BattleEventHandlerEntry BattleEventHandlerEntry;
typedef struct BattleEventItem BattleEventItem;
typedef struct BattleHandlerString BattleHandlerString;
typedef struct BattleMon BattleMon;
typedef struct BattleMoveCore BattleMoveCore;
typedef struct BattleMoveWork BattleMoveWork;
typedef struct BattleParty BattleParty;
typedef struct BoxPkmCreateParams BoxPkmCreateParams;
typedef struct BtlMainModule BtlMainModule;
typedef struct BtlClient BtlClient;
typedef struct BtlField BtlField;
typedef struct BtlPokeCon BtlPokeCon;
typedef struct BtlServer BtlServer;
typedef struct BtlServerFlow BtlServerFlow;
typedef struct BtlvCore BtlvCore;
typedef struct BtlvEffectSetup BtlvEffectSetup;
typedef struct BtlvFingerCursor BtlvFingerCursor;
typedef struct BtlvScu BtlvScu;
typedef struct BtlvStringParam BtlvStringParam;
typedef struct CasteliaRush CasteliaRush;
typedef struct EventData EventData;
typedef struct EventPokeCenHealData EventPokeCenHealData;
typedef struct FieldActorAnmProc FieldActorAnmProc;
typedef struct FieldPlayerCore FieldPlayerCore;
typedef struct FieldPlayerGrid FieldPlayerGrid;
typedef struct FieldPropInstance FieldPropInstance;
typedef struct FieldPropResAnmHeader FieldPropResAnmHeader;
typedef struct FieldPropResBundle FieldPropResBundle;
typedef struct FieldPropResInfo FieldPropResInfo;
typedef struct FieldPropResInstance FieldPropResInstance;
typedef struct FieldPropRTCState FieldPropRTCState;
typedef struct FieldScriptSupervisor FieldScriptSupervisor;
typedef struct FieldSound FieldSound;
typedef struct G3DTextDrawResource G3DTextDrawResource;
typedef struct HIDInputState HIDInputState;
typedef struct KeypadManager KeypadManager;
typedef struct GameSystemProcData GameSystemProcData;
typedef struct HighLinkSave HighLinkSave;
typedef struct ISSCitySys ISSCitySys;
typedef struct ISSCityUnit ISSCityUnit;
typedef struct ISSDungeonSys ISSDungeonSys;
typedef struct ISSRoadSys ISSRoadSys;
typedef struct ISSSwitchSys ISSSwitchSys;
typedef struct ISSSwitchSet ISSSwitchSet;
typedef struct ISSZoneSys ISSZoneSys;
typedef struct MapMatrix MapMatrix;
typedef struct MapMatrixFileHeader MapMatrixFileHeader;
typedef struct MapReplace MapReplace;
typedef struct MapReplaceEvent MapReplaceEvent;
typedef struct NetSyncPacket NetSyncPacket;
typedef struct PlayerVolumeFader PlayerVolumeFader;
typedef struct PlayerSave PlayerSave;
typedef struct RailLoader RailLoader;
typedef struct RecordSave RecordSave;
typedef struct RingtoneSys RingtoneSys;
typedef struct ScriptSubwork ScriptSubwork;
typedef struct ShortcutSave ShortcutSave;
typedef struct StadiumTrainerEntry StadiumTrainerEntry;
typedef struct SurveyProbabilityState SurveyProbabilityState;
typedef struct SurveyTextWork SurveyTextWork;
typedef struct TouchpadManager TouchpadManager;
typedef struct TPokeData TPokeData;
typedef struct TimeSigSave TimeSigSave;
typedef struct TrialHouseWork TrialHouseWork;
typedef struct WarpSequence WarpSequence;
typedef struct ZoneNPC ZoneNPC;
typedef struct ZoneTrigger ZoneTrigger;
typedef struct ZoneWarp ZoneWarp;

typedef struct CodeInParam CodeInParam;

typedef struct CommPlayerSupport CommPlayerSupport;

typedef struct GameManualEventWork GameManualEventWork;
typedef struct GameManualSubwork GameManualSubwork;

typedef struct ElScoreboard ElScoreboard;
typedef struct ElScoreboardPaletteTarget ElScoreboardPaletteTarget;

typedef struct Queue Queue;

typedef struct RespawnZoneInfo RespawnZoneInfo;

typedef struct NetSyncCommand NetSyncCommand;

typedef struct Config Config;
typedef struct PlayTime PlayTime;

typedef struct BAppCursor BAppCursor;
typedef struct BagItem BagItem;
typedef struct BGWinFrame BGWinFrame;
typedef struct BBagWork BBagWork;
typedef struct CursorMove CursorMove;

typedef struct StaActBg StaActBg;

typedef struct FieldFollowWk FieldFollowWk;

typedef struct MoveReminderProcessData MoveReminderProcessData;

typedef struct StaActEffect StaActEffect;
typedef struct StaActEffectSys StaActEffectSys;

typedef struct FieldDynCameraFunctions FieldDynCameraFunctions;
typedef struct RailDataHandle RailDataHandle;

typedef struct Event3DDemoWork Event3DDemoWork;

typedef struct BGFont BGFont;

typedef struct BtlActionState BtlActionState;

typedef struct ZoneNPCGridPosition ZoneNPCGridPosition;
typedef struct ZoneNPCRailPosition ZoneNPCRailPosition;

typedef struct PMSWordSave PMSWordSave;

typedef struct StaActObjSys StaActObjSys;

typedef struct FieldG3DObjResRequest FieldG3DObjResRequest;

typedef struct ActorPalSlots ActorPalSlots;

typedef struct ISSSwitch ISSSwitch;

typedef struct GameBeacon GameBeacon;
typedef struct GameBeaconSystem GameBeaconSystem;
typedef struct FestMissionConfig FestMissionConfig;
typedef struct FestMissionEntry FestMissionEntry;
typedef struct MedalBox MedalBox;
typedef struct LinkFestival LinkFestival;

typedef struct BtlAdapter BtlAdapter;

typedef struct MusicalItemData MusicalItemData;

typedef struct PCSubprocessEventData PCSubprocessEventData;

typedef struct TrainerLogSave TrainerLogSave;

// Save block 0x34, which getTrainerGameInfoAddress, getTrainerCardData_wrapper and getTrainerCardDataBlkAddress return
typedef struct SystemUI SystemUI;

typedef struct EncounterCutinWork EncounterCutinWork;

typedef struct BtlFieldStatus BtlFieldStatus;

typedef struct PlayerActionPerms PlayerActionPerms;

typedef struct DendouPCParam DendouPCParam;

typedef struct BrCodeInParam BrCodeInParam;

typedef struct BrBtnRecovery BrBtnRecovery;
typedef struct BrBtnSys BrBtnSys;
typedef struct BrRecordInfo BrRecordInfo;

typedef struct AppTaskMenuWin AppTaskMenuWin;
typedef struct PMSDraw PMSDraw;

typedef struct TrainerLogEntry TrainerLogEntry;

typedef struct BattleRecHeader BattleRecHeader;

typedef struct BSubwayPlayData BSubwayPlayData;
typedef struct BSubwayScoreData BSubwayScoreData;

typedef struct NetSave NetSave;
typedef struct RivalDataSave RivalDataSave;
typedef struct UnionApp UnionApp;
typedef struct PMSWordBank PMSWordBank;

typedef struct ZukanSearchParam ZukanSearchParam;
typedef struct BlinkPalAnm BlinkPalAnm;

typedef struct FieldmapCtrlHybrid FieldmapCtrlHybrid;

typedef struct MysteryGiftSave MysteryGiftSave;
typedef struct SaveOutside SaveOutside;


typedef struct FrameListTouch FrameListTouch;

typedef union WF2DMAP_MAP WF2DMAP_MAP;

typedef struct SpPokeGimmickNpc SpPokeGimmickNpc;

typedef struct FieldActorSave FieldActorSave;
typedef struct FieldActorMoveCode FieldActorMoveCode;
typedef struct FieldActorSceneNodeVTable FieldActorSceneNodeVTable;
typedef struct StrengthRockSave StrengthRockSave;

typedef struct PassPowerLevel PassPowerLevel;
typedef struct BeaconStatus BeaconStatus;
typedef struct BSubwayScrWork BSubwayScrWork;
typedef struct MusicalCommWork MusicalCommWork;
typedef struct PalPadFriendList PalPadFriendList;
typedef struct PokewoodSystem PokewoodSystem;
typedef struct ResortWork ResortWork;
typedef struct WbtSystem WbtSystem;

typedef struct DreamRadarSave DreamRadarSave;

typedef struct PerapVoice PerapVoice;

typedef struct DWCApInfo DWCApInfo;
typedef struct CtvtCommBeacon CtvtCommBeacon;

typedef struct PersonalData PersonalData;

typedef struct BrOutlineData BrOutlineData;
typedef struct BrListPos BrListPos;
typedef struct BrListItem BrListItem;
typedef struct BrList BrList;

typedef struct Ov210Work Ov210Work;

typedef struct LiveBattleMatchResult LiveBattleMatchResult;
typedef struct LiveBattleMatchPlayerData LiveBattleMatchPlayerData;

typedef struct MusItemDraw MusItemDraw;
typedef struct DupFittingItemList DupFittingItemList;
typedef struct MusPokeDraw MusPokeDraw;
typedef struct MusItemDrawSys MusItemDrawSys;
typedef struct MusPokeDrawSys MusPokeDrawSys;

typedef struct KeyInfoSave KeyInfoSave;

typedef struct HallOfFameSave HallOfFameSave;
typedef struct BrBtnDataSys BrBtnDataSys;

typedef struct RegulationUnkAC RegulationUnkAC;
typedef struct BtlSetupTrainer BtlSetupTrainer;

typedef struct BtlvInput BtlvInput;
typedef struct BtlvInputKeyStop BtlvInputKeyStop;

typedef struct Demo3DEngine Demo3DEngine;

typedef struct Demo3DSetup Demo3DSetup;

typedef struct CDemoParam CDemoParam;
typedef struct CDemoPalette CDemoPalette;
typedef struct CDemoWork CDemoWork;

typedef struct ZukanListParam ZukanListParam;

typedef struct ResearchListRecovery ResearchListRecovery;

typedef struct BeaconViewMenu BeaconViewMenu;
typedef struct FestivalText FestivalText;
typedef struct FestMissionEventArgs FestMissionEventArgs;

typedef struct FestMission FestMission;

typedef struct CGearSettings CGearSettings;

typedef struct BeaconDetailChars BeaconDetailChars;
typedef struct BeaconDetailObjRes BeaconDetailObjRes;
typedef struct BeaconDetailPalette BeaconDetailPalette;
typedef struct BeaconDetailPanel BeaconDetailPanel;
typedef struct BeaconDetailWindow BeaconDetailWindow;
typedef struct BeaconDetailParam BeaconDetailParam;
typedef struct BeaconDetailWork BeaconDetailWork;
typedef struct WbtTrainers WbtTrainers;

typedef struct UnSelectParam UnSelectParam;
typedef struct BrBvDeleteProcParam BrBvDeleteProcParam;

typedef struct BrBtlSubwayProcParam BrBtlSubwayProcParam;

typedef struct ThAwardParam ThAwardParam;
typedef struct TrialHousePokemon TrialHousePokemon;
typedef struct TrialHouseRecord TrialHouseRecord;
typedef struct TrialHouseSave TrialHouseSave;
typedef struct ManualTopParam ManualTopParam;
typedef struct MailParam MailParam;
typedef struct CommBtlDemoParam CommBtlDemoParam;

typedef struct VsMultiListParam VsMultiListParam;
typedef struct DDemoWork DDemoWork;
typedef struct DendouDemoParam DendouDemoParam;
typedef struct DDemoFontOam DDemoFontOam;
typedef struct CommTvtParam CommTvtParam;
typedef struct HallOfFameTeam HallOfFameTeam;

typedef struct IrcCompatibleParam0 IrcCompatibleParam0;
typedef struct PselParam PselParam;
typedef struct WF_2DC_WKDATA WF_2DC_WKDATA;

typedef struct ZukanAwardParam ZukanAwardParam;
typedef struct OekakiMember OekakiMember;
typedef struct OekakiOldTouch OekakiOldTouch;
typedef struct OekakiParam OekakiParam;
typedef struct OekakiQueue OekakiQueue;
typedef struct OekakiTouchInfo OekakiTouchInfo;
typedef struct OekakiResource OekakiResource;
typedef struct OekakiSignal OekakiSignal;
typedef struct OekakiTouchData OekakiTouchData;
typedef struct OekakiWork OekakiWork;
typedef struct OekakiPictureChunk OekakiPictureChunk;
typedef struct BattleHandlerMessageParam BattleHandlerMessageParam;

typedef struct GSyncDownload GSyncDownload;

typedef struct SurveyProbabilityEntry SurveyProbabilityEntry;

typedef struct PercentageResources PercentageResources;
typedef struct Percentage Percentage;

typedef struct CtvtMic CtvtMic;

typedef struct BSubwayPokemonData BSubwayPokemonData;
typedef struct BSubwayPokemon BSubwayPokemon;
typedef struct BSubwayTeamConfig BSubwayTeamConfig;

typedef struct DeliveryInit DeliveryInit;

typedef struct GdsRapCallbacks GdsRapCallbacks;
typedef struct GdsRapResult GdsRapResult;
typedef struct GdsRapInit GdsRapInit;
typedef struct ColosseumMember ColosseumMember;
typedef struct UnionBeacon UnionBeacon;
typedef struct UnionMember UnionMember;
typedef struct UnionMemberList UnionMemberList;
typedef struct ColosseumChoice ColosseumChoice;
typedef struct CommPlayerStatus CommPlayerStatus;
typedef struct CommPlayerSys CommPlayerSys;
typedef struct UnionColosseum UnionColosseum;
typedef struct CommEntryMenu CommEntryMenu;

typedef struct BrPokeSearch BrPokeSearch;

typedef struct BrMusicalShotRecv BrMusicalShotRecv;
typedef struct BSubwayRecordsParam BSubwayRecordsParam;
typedef struct BrRecordProcParam BrRecordProcParam;
typedef struct BrProfile BrProfile;
typedef struct HallOfFameSavePokemon HallOfFameSavePokemon;
typedef struct BootScreensParam BootScreensParam;

typedef struct BtlvEffToolMove BtlvEffToolMove;
typedef struct BtlvBg BtlvBg;

typedef struct IrcBattleMatchParam IrcBattleMatchParam;
typedef struct WifiListFriend WifiListFriend;

typedef struct ColosseumPlayers ColosseumPlayers;
typedef struct UnionSelf UnionSelf;
typedef struct UnionBeaconEntry UnionBeaconEntry;
typedef struct UnionActorSlot UnionActorSlot;
typedef struct UnionVisitorList UnionVisitorList;
typedef struct UnionVisitor UnionVisitor;
typedef struct UnionSubproc UnionSubproc;
typedef struct UnionBootParam UnionBootParam;
typedef struct UnionGroup UnionGroup;
typedef struct UnionSystem UnionSystem;
typedef struct Demo3DFiles Demo3DFiles;
typedef struct Demo3DUnit Demo3DUnit;
typedef struct Demo3DVariant Demo3DVariant;

typedef struct Demo3DCaptureParam Demo3DCaptureParam;
typedef struct WifiBattleMatchNetCupStats WifiBattleMatchNetCupStats;
typedef struct WifiBattleMatchNetRecordData WifiBattleMatchNetRecordData;
typedef struct WifiBattleMatchNetStats WifiBattleMatchNetStats;

typedef struct ListMenuRequest ListMenuRequest;
typedef struct RegulationRulesText RegulationRulesText;

typedef struct PMSSelectParam PMSSelectParam;
typedef struct UnionColosseumParam UnionColosseumParam;
typedef struct MoveConditionParam MoveConditionParam;

typedef struct GtsNegoScroll GtsNegoScroll;
typedef struct WifiBattleMatchRankInfo WifiBattleMatchRankInfo;

typedef struct TradedPokemonSave TradedPokemonSave;

typedef struct WifiBattleMatchBattleParam WifiBattleMatchBattleParam;
typedef struct WifiBattleMatchPlayers WifiBattleMatchPlayers;
typedef struct UnionAppStatus UnionAppStatus;

typedef struct CtvtGameMember CtvtGameMember;
typedef struct CameraSystem CameraSystem;
typedef struct CtvtGameCam CtvtGameCam;
typedef struct CtvtGameCamGraphic CtvtGameCamGraphic;
typedef struct CtvtGameCamTask CtvtGameCamTask;
typedef struct ProfWordLists ProfWordLists;
typedef struct DrawCommand DrawCommand;
typedef struct DrawSystem DrawSystem;
typedef struct DrawSystemParam DrawSystemParam;
typedef struct StaActLight StaActLight;
typedef struct StaActLightSys StaActLightSys;
typedef struct StaActPoke StaActPoke;
typedef struct StaActPokeSys StaActPokeSys;
typedef struct BrCoreParam BrCoreParam;
typedef struct BrData BrData;
typedef struct BrProcSysRecovery BrProcSysRecovery;
typedef struct BrBtnStack BrBtnStack;

typedef struct EventActorAnmProcWaitWork EventActorAnmProcWaitWork;

typedef struct FieldEvCameraShake FieldEvCameraShake;

typedef struct EncEffCaptureWork EncEffCaptureWork;

typedef struct FieldCameraCoords FieldCameraCoords;

typedef struct SymbolMapList SymbolMapList;

typedef struct ZukanDetailBGSetup ZukanDetailBGSetup;

typedef struct PokewoodSave PokewoodSave;

typedef struct PokeListMenu PokeListMenu;
typedef struct PokeListMessage PokeListMessage;
typedef struct PokeListPartner PokeListPartner;
typedef struct PokeListPlate PokeListPlate;
typedef struct PokeListWork PokeListWork;

typedef struct BtlRecReader BtlRecReader;
typedef struct BtlRecTool BtlRecTool;
typedef struct BtlRecorder BtlRecorder;

typedef struct MysteryGraphic MysteryGraphic;

typedef struct MusicalShotParam MusicalShotParam;
typedef struct MusShotInfo MusShotInfo;

typedef struct BSubwayBeacon BSubwayBeacon;
typedef struct Ov306Param Ov306Param;

typedef struct ResearchActorSetup ResearchActorSetup;
typedef struct ResearchPaletteAnimeSetup ResearchPaletteAnimeSetup;
typedef struct ResearchRect ResearchRect;
typedef struct PaletteAnime PaletteAnime;
typedef struct ResearchCommon ResearchCommon;

typedef struct Box2FontOam Box2FontOam;
typedef struct Box2IrqWork Box2IrqWork;
typedef struct Box2ButtonAnm Box2ButtonAnm;
typedef struct Box2RangeSelect Box2RangeSelect;

typedef struct BtlvBGaugeParam BtlvBGaugeParam;
typedef struct BtlvBGauge BtlvBGauge;

typedef struct MusicalMcss MusicalMcss;
typedef struct MusicalMcssSys MusicalMcssSys;

typedef struct BattleParam BattleParam;
typedef struct BattleProcWork BattleProcWork;
typedef struct EventMakeWork EventMakeWork;

typedef struct PStaOam PStaOam;
typedef struct PStaOamActor PStaOamActor;

typedef struct DayCarePokeSelectWork DayCarePokeSelectWork;

typedef struct UnovaLinkParam UnovaLinkParam;
typedef struct GameClearWork GameClearWork;

typedef struct IntroMenuItem IntroMenuItem;

typedef struct ChatotEventWork ChatotEventWork;

typedef struct ResearchGraph ResearchGraph;
typedef struct ResearchList ResearchList;
typedef struct ResearchTop ResearchTop;

typedef struct BtlvTimer BtlvTimer;

typedef struct FieldScriptEnvArgs FieldScriptEnvArgs;

typedef struct StaActButton StaActButton;

typedef struct DemoParticle DemoParticle;
typedef struct DemoParticleEvent DemoParticleEvent;
typedef struct DemoParticleUnit DemoParticleUnit;
typedef struct DemoParticleResource DemoParticleResource;

typedef struct Box2MenuItem Box2MenuItem;
typedef struct Box2PokeInfo Box2PokeInfo;

typedef struct PokemonTradeGtsParam PokemonTradeGtsParam;

typedef struct EncEffRipple EncEffRipple;

typedef struct EventFieldTradeWork EventFieldTradeWork;
typedef struct FieldTradeInput FieldTradeInput;
typedef struct FieldTradeOfferData FieldTradeOfferData;

typedef struct CommPlayer CommPlayer;
typedef struct CommPlayerSent CommPlayerSent;

typedef struct BtlvClact BtlvClact;

typedef struct BtlvCamera BtlvCamera;

typedef struct StaActAudienceMember StaActAudienceMember;
typedef struct StaActAudience StaActAudience;

typedef struct ArrowResources ArrowResources;
typedef struct Arrow Arrow;

typedef struct ItemParams ItemParams;

typedef struct MBCommParentInfo MBCommParentInfo;
typedef struct MBCommSys MBCommSys;

typedef struct PdwAccParam PdwAccParam;
typedef struct PdwAccDisp PdwAccDisp;
typedef struct PdwAccMessage PdwAccMessage;

typedef struct EventSeasonBanner EventSeasonBanner;

typedef struct MonolithActorRes MonolithActorRes;

typedef struct WifiBattleMatchExchangeParam WifiBattleMatchExchangeParam;
typedef struct WifiBattleMatchListParam WifiBattleMatchListParam;

typedef struct TrialHouseEventData TrialHouseEventData;
typedef struct TrialHouseEffectEvent TrialHouseEffectEvent;

typedef struct HiddenEventSpec HiddenEventSpec;
typedef struct HiddenArea HiddenArea;
typedef struct HiddenEventData HiddenEventData;

typedef struct TrainerData TrainerData;
typedef struct TrainerPoke TrainerPoke;
typedef struct TrainerPokeItem TrainerPokeItem;
typedef struct TrainerPokeItemMoves TrainerPokeItemMoves;
typedef struct TrainerPokeMoves TrainerPokeMoves;

typedef struct Elboard Elboard;
typedef struct ElboardInit ElboardInit;
typedef struct ElboardMessage ElboardMessage;
typedef struct ElboardMessageArg ElboardMessageArg;
typedef struct GimmickGateBoardEntry GimmickGateBoardEntry;
typedef struct GimmickGateEventData GimmickGateEventData;

typedef struct BattleMonStatStages BattleMonStatStages;
typedef struct BattleMonCore BattleMonCore;
typedef struct BattleMonLevelUp BattleMonLevelUp;

typedef struct BtlCommTrainerData BtlCommTrainerData;
typedef struct BtlMainSeq BtlMainSeq;
typedef struct BtlMainUnk478 BtlMainUnk478;
typedef struct BtlTrainerData BtlTrainerData;
typedef struct BtlClientIDList BtlClientIDList;
typedef struct BtlScriptedRules BtlScriptedRules;
typedef struct AdjacentOpponentData AdjacentOpponentData;
typedef struct BtlMainSyncData BtlMainSyncData;
typedef struct PokewoodCutin PokewoodCutin;

typedef struct AbilityEventAddEntry AbilityEventAddEntry;
typedef struct BattleHandlerAbilityChangeParam BattleHandlerAbilityChangeParam;
typedef struct BattleHandlerAddConditionParam BattleHandlerAddConditionParam;
typedef struct BattleHandlerChangeWeatherParam BattleHandlerChangeWeatherParam;
typedef struct BattleHandlerChangeFormParam BattleHandlerChangeFormParam;
typedef struct BattleHandlerChangeHPParam BattleHandlerChangeHPParam;
typedef struct BattleHandlerChangeTypeParam BattleHandlerChangeTypeParam;
typedef struct BattleHandlerCheckHeldItemParam BattleHandlerCheckHeldItemParam;
typedef struct BattleHandlerCureConditionParam BattleHandlerCureConditionParam;
typedef struct BattleHandlerDamageParam BattleHandlerDamageParam;
typedef struct BattleHandlerRecoverHPParam BattleHandlerRecoverHPParam;
typedef struct BattleHandlerFlagParam BattleHandlerFlagParam;
typedef struct BattleHandlerIllusionBreakParam BattleHandlerIllusionBreakParam;
typedef struct BattleHandlerSetItemParam BattleHandlerSetItemParam;
typedef struct BattleHandlerStatChangeParam BattleHandlerStatChangeParam;
typedef struct BattleHandlerSwapItemParam BattleHandlerSwapItemParam;
typedef struct BattleHandlerTransformParam BattleHandlerTransformParam;

typedef struct BtlWazaRec BtlWazaRec;
typedef struct BtlFaintRec BtlFaintRec;
typedef struct BattleHandlerSetStatStageParam BattleHandlerSetStatStageParam;
typedef struct BattleHandlerPosEffectParam BattleHandlerPosEffectParam;
typedef struct BattleHandlerUpdateMoveParam BattleHandlerUpdateMoveParam;
typedef struct BattleHandlerForceSwitchParam BattleHandlerForceSwitchParam;
typedef struct BattleHandlerEffectAtPosParam BattleHandlerEffectAtPosParam;
typedef struct BattleHandlerAddSideEffectParam BattleHandlerAddSideEffectParam;
typedef struct BattleHandlerRemoveSideEffectParam BattleHandlerRemoveSideEffectParam;
typedef struct BattleHandlerSetStatusParam BattleHandlerSetStatusParam;
typedef struct BattleHandlerAddFieldEffectParam BattleHandlerAddFieldEffectParam;
typedef struct BattleHandlerConsumeItemParam BattleHandlerConsumeItemParam;
typedef struct BattleHandlerPPParam BattleHandlerPPParam;
typedef struct BattleHandlerFaintParam BattleHandlerFaintParam;
typedef struct BattleHandlerFlinchParam BattleHandlerFlinchParam;
typedef struct BattleHandlerForceUseItemParam BattleHandlerForceUseItemParam;
typedef struct BattleHandlerHideTurnParam BattleHandlerHideTurnParam;
typedef struct BattleHandlerInterruptParam BattleHandlerInterruptParam;
typedef struct BattleHandlerMoveEffectParam BattleHandlerMoveEffectParam;
typedef struct BattleHandlerResetStatStageParam BattleHandlerResetStatStageParam;
typedef struct BattleHandlerRemoveFieldEffectParam BattleHandlerRemoveFieldEffectParam;
typedef struct BattleHandlerSetWeightParam BattleHandlerSetWeightParam;
typedef struct BattleHandlerSwitchParam BattleHandlerSwitchParam;
typedef struct BattleHandlerSwapPokeParam BattleHandlerSwapPokeParam;
typedef struct BattleHandlerSetCounterParam BattleHandlerSetCounterParam;

typedef struct BtlStudioTurnEvent BtlStudioTurnEvent;
typedef struct BtlStudioScene BtlStudioScene;
typedef struct BtlStudioChoice BtlStudioChoice;
typedef struct BtlServerCmdQueue BtlServerCmdQueue;
typedef struct BtlvMcss BtlvMcss;
typedef struct BtlvPokeListCmd BtlvPokeListCmd;
typedef struct BtlvPokeSelectParam BtlvPokeSelectParam;
typedef struct BtlvSelectTargetParam BtlvSelectTargetParam;

typedef struct BtlFlowCalledMove BtlFlowCalledMove;
typedef struct BtlFlowClientList BtlFlowClientList;
typedef struct BtlFlowDamageEntry BtlFlowDamageEntry;
typedef struct BtlFlowDamageFlags BtlFlowDamageFlags;
typedef struct BtlFlowDamageList BtlFlowDamageList;
typedef struct BtlFlowExpEntry BtlFlowExpEntry;
typedef struct BtlFlowFightWork BtlFlowFightWork;
typedef struct BtlFlowHitWork BtlFlowHitWork;
typedef struct BtlFlowMonIter BtlFlowMonIter;
typedef struct BtlFlowReactionList BtlFlowReactionList;
typedef struct BtlFlowUnk1B50 BtlFlowUnk1B50;
typedef struct BtlFlowWorkFrame BtlFlowWorkFrame;
typedef struct BtlWazaRecord BtlWazaRecord;
typedef struct BtlFaintRecTurn BtlFaintRecTurn;
typedef struct BtlPosPoke BtlPosPoke;
typedef struct BtlPosPokeEntry BtlPosPokeEntry;
typedef struct BtlClientActions BtlClientActions;
typedef struct BtlMonSet BtlMonSet;
typedef struct BattleHandlerDelayMoveDamageParam BattleHandlerDelayMoveDamageParam;
typedef struct BattleHandlerBatonPassParam BattleHandlerBatonPassParam;
typedef struct BattleHandlerDrainParam BattleHandlerDrainParam;
typedef struct BattleHandlerGravityCheckParam BattleHandlerGravityCheckParam;
typedef struct BattleHandlerUseHeldItemParam BattleHandlerUseHeldItemParam;
typedef struct BattleHandlerPopupParam BattleHandlerPopupParam;
typedef struct BattleHandlerQuitBattleParam BattleHandlerQuitBattleParam;
typedef struct BattleHandlerRecoverStatStageParam BattleHandlerRecoverStatStageParam;
typedef struct BattleHandlerReviveParam BattleHandlerReviveParam;
typedef struct BattleMoveEffectState BattleMoveEffectState;

typedef struct StaActPokeItemWork StaActPokeItemWork;

typedef struct ResSprite ResSprite;

typedef struct PokemonTradeDemoParam PokemonTradeDemoParam;
typedef struct TradeBoxEntry TradeBoxEntry;

typedef struct TradeBoxColors TradeBoxColors;
typedef struct TradeCurve TradeCurve;
typedef struct TradeMarkIcons TradeMarkIcons;

typedef struct FieldMenu FieldMenu;

typedef struct ActorTerrainEffectParam ActorTerrainEffectParam;

typedef struct NPokeSpec NPokeSpec;

typedef struct PStaSubWork PStaSubWork;

typedef struct BtlvMcssCircle BtlvMcssCircle;
typedef struct BtlvMcssShake BtlvMcssShake;
typedef struct BtlvGauge BtlvGauge;
typedef struct BtlvStage BtlvStage;

typedef struct MusicalEventWork MusicalEventWork;

typedef struct WipeHBlank WipeHBlank;
typedef struct WipeScreen WipeScreen;

typedef struct IntroCmdEntry IntroCmdEntry;

typedef struct PMSIVCategory PMSIVCategory;
typedef struct PMSIVEdit PMSIVEdit;
typedef struct PMSIVMenu PMSIVMenu;

typedef struct MysteryCardResSetup MysteryCardResSetup;
typedef struct MysteryCardViewSetup MysteryCardViewSetup;
typedef struct MysteryTextWinUpdate MysteryTextWinUpdate;
typedef struct MysteryYesNoSetup MysteryYesNoSetup;
typedef struct MysteryTextWinEntry MysteryTextWinEntry;
typedef struct MysteryAlbum MysteryAlbum;
typedef struct MysteryCardRes MysteryCardRes;
typedef struct MysteryCardView MysteryCardView;
typedef struct MysteryMsgWin MysteryMsgWin;
typedef struct MysterySeq MysterySeq;
typedef struct MysteryTextLine MysteryTextLine;
typedef struct MysteryTextWin MysteryTextWin;
typedef struct MysteryTextWinCopy MysteryTextWinCopy;
typedef struct MysteryYesNo MysteryYesNo;

typedef struct CircleGraphData CircleGraphData;
typedef struct CircleGraph CircleGraph;

typedef struct GimmickGateMessageList GimmickGateMessageList;
typedef struct GimmickGateSave GimmickGateSave;
typedef struct GimmickGateWork GimmickGateWork;
typedef struct GimmickGateZoneData GimmickGateZoneData;
typedef struct GimmickGateZoneList GimmickGateZoneList;

typedef struct FieldPropDoorSounds FieldPropDoorSounds;
typedef struct FieldPropPosition FieldPropPosition;
typedef struct FieldPropResBank FieldPropResBank;
typedef struct FieldPropResInstanceHandle FieldPropResInstanceHandle;
typedef struct FieldPropResource FieldPropResource;

typedef struct ScriptActorWork ScriptActorWork;

typedef struct FieldChunkLocator FieldChunkLocator;
typedef struct FieldTerrainAnmInfo FieldTerrainAnmInfo;
typedef struct FieldChunkConfig FieldChunkConfig;
typedef struct FieldChunkVTable FieldChunkVTable;
typedef struct FieldChunkHandle FieldChunkHandle;
typedef struct FieldG3DMapperTextureFile FieldG3DMapperTextureFile;
typedef struct FieldG3DMapperConfig FieldG3DMapperConfig;
typedef struct HeightExList HeightExList;
// Black's FieldG3DMapper.resortMap holds height_ex.c's list: Black 2's name for its type, until that field is renamed
typedef struct HeightExList ResortMapCreateWork;

typedef struct GSyncDisp GSyncDisp;

typedef struct MysteryListSetup MysteryListSetup;
typedef struct MysteryList MysteryList;
typedef struct MysteryNet MysteryNet;
typedef struct MysteryOamText MysteryOamText;

typedef struct CommTvtWork CommTvtWork;
typedef struct CtvtCall CtvtCall;
typedef struct CtvtCamera CtvtCamera;
typedef struct CtvtComm CtvtComm;

typedef struct CtvtDraw CtvtDraw;
typedef struct CtvtTalk CtvtTalk;

typedef struct CtvtCommMemberInfo CtvtCommMemberInfo;
typedef struct CtvtGame CtvtGame;

typedef struct EventEntralinkWarp EventEntralinkWarp;
typedef struct EventFieldContinue EventFieldContinue;
typedef struct EventFieldFirst EventFieldFirst;
typedef struct EventGameOpening EventGameOpening;
typedef struct EventMapChange EventMapChange;
typedef struct EventMapChangeBlackout EventMapChangeBlackout;
typedef struct EventMapChangeCore EventMapChangeCore;
typedef struct FieldLensFlare FieldLensFlare;
typedef struct ZoneGimmick ZoneGimmick;

typedef struct CtvtVoicePacket CtvtVoicePacket;

typedef struct BtlvInputCommandParam BtlvInputCommandParam;
typedef struct BtlvInputPokeEntry BtlvInputPokeEntry;
typedef struct BtlvInputMoveParam BtlvInputMoveParam;
typedef struct BtlvInputRecorderParam BtlvInputRecorderParam;
typedef struct BtlvInputRotationParam BtlvInputRotationParam;
typedef struct BtlvInputTargetEntry BtlvInputTargetEntry;
typedef struct BtlvInputTargetParam BtlvInputTargetParam;
typedef struct BtlvEffect BtlvEffect;

typedef struct Box2PokeFreeWork Box2PokeFreeWork;
typedef struct Box2SearchParam Box2SearchParam;

typedef struct Box2Area Box2Area;
typedef struct Box2CursorMoveWork Box2CursorMoveWork;
typedef struct Box2ItemMoveWork Box2ItemMoveWork;
typedef struct Box2NameInWork Box2NameInWork;
typedef struct Box2PokeMoveData Box2PokeMoveData;
typedef struct Box2PokeMoveWork Box2PokeMoveWork;
typedef struct BoxSearchParam BoxSearchParam;
typedef struct BagProcessData BagProcessData;

typedef struct MBParentParam MBParentParam;

typedef struct BagItemListEntry BagItemListEntry;
typedef struct BagItemList BagItemList;
typedef struct ItemMenuWork ItemMenuWork;

typedef struct BtlvSubProc BtlvSubProc;
typedef struct BtlvScd BtlvScd;

typedef struct ZukanDetailFormActorData ZukanDetailFormActorData;
typedef struct ZukanDetailFormParam ZukanDetailFormParam;
typedef struct ZukanDetailFormPos ZukanDetailFormPos;
typedef struct ZukanDetailWindowData ZukanDetailWindowData;

typedef struct ZukanDetailVoiceParam ZukanDetailVoiceParam;

typedef struct BtlvMoveEffectParam BtlvMoveEffectParam;

typedef struct BtlvEffvmParam BtlvEffvmParam;

typedef struct BtlvEffvm BtlvEffvm;

typedef struct MarineTubeBoardParam MarineTubeBoardParam;
typedef struct ScriptProcCallbackWork ScriptProcCallbackWork;
typedef struct BagScriptResult BagScriptResult;
typedef struct MailboxProcessData MailboxProcessData;
typedef struct ScriptOverlayWork ScriptOverlayWork;

typedef struct EntranceCameraParam EntranceCameraParam;
typedef struct EntranceCamera EntranceCamera;
typedef struct EntranceCameraRecord EntranceCameraRecord;

typedef struct KeyDataSave KeyDataSave;

typedef struct ShortcutMenuWork ShortcutMenuWork;

typedef struct EncountManager EncountManager;
typedef struct WildPkmParam WildPkmParam;
typedef struct EncountRateConfig EncountRateConfig;

typedef struct BrBvSearchProcParam BrBvSearchProcParam;
typedef struct BrMusicalLookProcParam BrMusicalLookProcParam;
typedef struct BrRndMatchProcParam BrRndMatchProcParam;
typedef struct BrProcData BrProcData;

typedef struct RareEncountSpot RareEncountSpot;

typedef struct WifiClubData WifiClubData;

typedef struct BattleEventVarStack BattleEventVarStack;

typedef struct HiddenHollow HiddenHollow;
typedef struct SaveData SaveData;

typedef struct PokewoodBattleInfo PokewoodBattleInfo;
typedef struct PokewoodBlock PokewoodBlock;

typedef struct WifiNoteParam WifiNoteParam;

typedef struct WifiBSubwayParam WifiBSubwayParam;

typedef struct IrcCompatibleParam4 IrcCompatibleParam4;

typedef struct IrcCompatibleParam1 IrcCompatibleParam1;

typedef struct MailBoxWork MailBoxWork;

typedef struct MailBoxPrintState MailBoxPrintState;
typedef struct MailBoxPaletteFx MailBoxPaletteFx;

typedef struct BattleExaminationSave BattleExaminationSave;

typedef struct TournamentBlock TournamentBlock;
typedef struct TournamentTeam TournamentTeam;

typedef struct IrcCompatibleParam IrcCompatibleParam;

typedef struct TradedPokemon TradedPokemon;

typedef struct ShortcutMenuApp ShortcutMenuApp;

typedef struct MemoryLinkSave MemoryLinkSave;
typedef struct MemoryLinkPlayer MemoryLinkPlayer;
typedef struct MemoryLinkMemories MemoryLinkMemories;

typedef struct ZukanDetailMapParam ZukanDetailMapParam;
typedef struct ZukanDetailParam ZukanDetailParam;
typedef struct ZukanDetailInfoParam ZukanDetailInfoParam;


typedef struct Box2BoxListDrag Box2BoxListDrag;

typedef struct FieldPlayerNoGrid FieldPlayerNoGrid;

typedef struct EggDemoParam EggDemoParam;

typedef struct BSubwayTrainerRange BSubwayTrainerRange;

typedef struct ZoneBGEntity ZoneBGEntity;
typedef struct ZoneWarpGridPosition ZoneWarpGridPosition;
typedef struct ZoneWarpRailPosition ZoneWarpRailPosition;

typedef struct PStaRibbonWork PStaRibbonWork;

typedef struct TradeBackup TradeBackup;

typedef struct FieldGimmickWorkBlock FieldGimmickWorkBlock;
typedef struct FieldmapCtrlVTable FieldmapCtrlVTable;

typedef struct FieldMsgBG FieldMsgBG;
typedef struct ActorMsgWin ActorMsgWin;
typedef struct FieldMsgBGWin FieldMsgBGWin;
typedef struct MsgBGCursor MsgBGCursor;
typedef struct MsgBGStream MsgBGStream;
typedef struct MsgBGSysWin MsgBGSysWin;
typedef struct MsgBGTalkWin MsgBGTalkWin;
typedef struct MsgBGBalloonWin MsgBGBalloonWin;
typedef struct MsgBGNameWin MsgBGNameWin;
typedef struct MsgBGScrollWin MsgBGScrollWin;
typedef struct MsgBGSignWin MsgBGSignWin;

typedef struct EventScriptCallData EventScriptCallData;
typedef struct GlobalScriptEntry GlobalScriptEntry;

typedef struct MonolithPanelControl MonolithPanelControl;

typedef struct PStaWindowSetup PStaWindowSetup;
typedef struct PStaSkillWork PStaSkillWork;

typedef struct WorldTradeInputHeader WorldTradeInputHeader;
typedef struct WorldTradePrint WorldTradePrint;
typedef struct WorldTradePrintEntry WorldTradePrintEntry;

typedef struct EventIRCWork EventIRCWork;

typedef struct PStaInfoWork PStaInfoWork;

typedef struct WorldTradeDepositWork WorldTradeDepositWork;
typedef struct WorldTradeSelectListPos WorldTradeSelectListPos;

typedef struct SOCHostEnt SOCHostEnt;
typedef struct SOCInAddr SOCInAddr;
typedef struct SOCSockAddrIn SOCSockAddrIn;
typedef struct SOCSockAddr SOCSockAddr;

#endif // BWPORT_STRUCT_DECLS_H
