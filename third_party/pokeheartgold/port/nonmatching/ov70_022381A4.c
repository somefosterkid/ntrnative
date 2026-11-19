#include "global.h"
struct GTSCountrySearchRequest;
struct GTSNetworkingState;
struct GTSPokemonCriteria;
struct GTSPokemonListing;
struct GTSPokemonListingMon;
struct GTSPokemonRequirements;
typedef struct GTSPokemonListingMon {
    u8 bytes[236];
} GTSPokemonListingMon;
typedef struct GTSPokemonCriteria {
    s16 species;
    s8 gender;
    s8 level;
} GTSPokemonCriteria;
typedef struct GTSPokemonRequirements {
    s16 species;
    s8 gender;
    s8 level;
    s8 level2;
    s8 unset_05;
} GTSPokemonRequirements;
typedef struct GTSPokemonListing {
    GTSPokemonListingMon pokemon;
    GTSPokemonCriteria criteria;
    GTSPokemonRequirements requirements;
    u8 trainerGender;
    u8 unused[17];
    s32 listingId;
    u16 trainerNames[8];
    u16 trainerId;
    u8 trainerCountry;
    u8 trainerRegion;
    u8 trainerAppearance;
    s8 exchangedFromRemote;
    u8 gameVersion;
    u8 trainerLanguage;
} GTSPokemonListing;
typedef struct GTSCountrySearchRequest {
    s16 species;
    s8 gender;
    s8 level;
    s8 level2;
    s8 unk_05;
    s8 count;
    u8 countryIndex;
} GTSCountrySearchRequest;
enum GTSNetworkResult {
    GTS_RESULT_SUCCESS = 0,
    GTS_RESULT_PROFILE_EXISTS = 1,
    GTS_RESULT_BANNED = 2,
    GTS_RESULT_1_FOUND = 1,
    GTS_RESULT_2_FOUND = 2,
    GTS_RESULT_3_FOUND = 3,
    GTS_RESULT_4_FOUND = 4,
    GTS_RESULT_5_FOUND = 5,
    GTS_RESULT_6_FOUND = 6,
    GTS_RESULT_7_FOUND = 7,

    GTS_RESULT_GTS_CROWDED = -1,
    GTS_RESULT_SERVER_MAINTENANCE = -2,
    GTS_RESULT_ALREADY_TRADED = -3,
    GTS_RESULT_NOT_FOUND = -4,
    GTS_RESULT_ALREADY_DEPOSITED = -5,
    GTS_RESULT_CANNOT_BE_OFFERED_1 = -6,
    GTS_RESULT_CANNOT_BE_OFFERED_2 = -7,
    GTS_RESULT_CANNOT_BE_OFFERED_3 = -8,
    GTS_RESULT_CANNOT_BE_OFFERED_4 = -9,
    GTS_RESULT_CANNOT_BE_OFFERED_5 = -10,
    GTS_RESULT_CANNOT_BE_OFFERED_6 = -11,
    GTS_RESULT_ABORTED = -12,
    GTS_RESULT_ERROR = -13,
    GTS_RESULT_NOT_IN_DCF_PHASE = -14,
    GTS_RESULT_CONNECTION_FAILED = -15,
};
void ov38_0221BE84(void);
void ov38_0221BFEC(void);
typedef enum GTSNetworkStatus {
    NETWORK_STATUS_INACTIVE,
    NETWORK_STATUS_IDLE,
    NETWORK_STATUS_POST_PENDING,
    NETWORK_STATUS_POST_PENDING_ABORTED,
    NETWORK_STATUS_POST_FINISH_PENDING,
    NETWORK_STATUS_POST_FINISH_PENDING_ABORTED,
    NETWORK_STATUS_GET_LISTED_POKEMON_PENDING,
    NETWORK_STATUS_GET_LISTED_POKEMON_PENDING_ABORTED,
    NETWORK_STATUS_GET_LISTING_STATUS_PENDING,
    NETWORK_STATUS_GET_LISTING_STATUS_PENDING_ABORTED,
    NETWORK_STATUS_DELETE_PENDING,
    NETWORK_STATUS_DELETE_PENDING_ABORTED,
    NETWORK_STATUS_RETURN_PENDING,
    NETWORK_STATUS_RETURN_PENDING_ABORTED,
    NETWORK_STATUS_SEARCH_PENDING,
    NETWORK_STATUS_SEARCH_PENDING_ABORTED,
    NETWORK_STATUS_EXCHANGE_PENDING,
    NETWORK_STATUS_EXCHANGE_PENDING_ABORTED,
    NETWORK_STATUS_EXCHANGE_FINISH_PENDING,
    NETWORK_STATUS_EXCHANGE_FINISH_PENDING_ABORTED,
    NETWORK_STATUS_INIT_PENDING,
    NETWORK_STATUS_INIT_PENDING_ABORTED,
    NETWORK_STATUS_SET_PROFILE_PENDING,
    NETWORK_STATUS_SET_PROFILE_PENDING_ABORTED,
    NETWORK_STATUS_COMPLETE
} GTSNetworkStatus;
typedef struct GTSNetworkingState {
    GTSNetworkStatus status;
    s32 result;
    s32 dwcProfileId;
    u64 friendKey;
    u8 requestData[296];
    u8 responseStatus[4];
    u8 *responseData;
} GTSNetworkingState;
BOOL ov70_02238360(const u8 *url, const void *data, int dataSize, void *response, int maxResponseLength);
extern GTSNetworkingState _02246800;

void ov70_022381A4(const GTSCountrySearchRequest *req, GTSPokemonListing *listing);
void ov70_022381A4(const GTSCountrySearchRequest *req, GTSPokemonListing *listing)
{
    _02246800.responseData = (u8 *)listing;

    ov38_0221BE84();

    memcpy(&_02246800.requestData[0], req, sizeof(GTSCountrySearchRequest));

    if (ov70_02238360((const u8 *)"http://gamestats2.gs.nintendowifi.net/pokemondpds/" "worldexchange/search.asp" , _02246800.requestData, sizeof(GTSCountrySearchRequest), listing, (int)sizeof(GTSPokemonListing) * req->count)) {
        _02246800.status = NETWORK_STATUS_SEARCH_PENDING;
    } else {
        _02246800.status = NETWORK_STATUS_COMPLETE;
        _02246800.result = GTS_RESULT_ERROR;
        ov38_0221BFEC();
    }
}
