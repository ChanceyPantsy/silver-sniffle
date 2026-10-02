// Social link roster. Add new links to include/constants/social_links.h first.
//
// Rank caps: a link can never go above its current cap, extra points are discarded.
// A link's cap is the highest rankCap among its gates whose flag is set, or baseRankCap if none are.
// Use any flag here (badges, story events, a FLAG_TEMP you set in a cutscene, ...).

// Story progression: one rank per badge, the last rank after entering the Hall of Fame.
static const struct SocialLinkRankGate sSocialLinkGates_Story[] =
{
    { FLAG_BADGE01_GET, 3 },
    { FLAG_BADGE02_GET, 4 },
    { FLAG_BADGE03_GET, 5 },
    { FLAG_BADGE04_GET, 6 },
    { FLAG_BADGE05_GET, 7 },
    { FLAG_BADGE06_GET, 8 },
    { FLAG_BADGE07_GET, 8 },
    { FLAG_BADGE08_GET, 9 },
    { FLAG_IS_CHAMPION, SOCIAL_LINK_MAX_RANK },
    { 0 },
};

#define GYM_LEADER_GATES(defeatedFlag)                      \
    (const struct SocialLinkRankGate[])                     \
    {                                                       \
        { defeatedFlag, 7 },                                \
        { FLAG_IS_CHAMPION, SOCIAL_LINK_MAX_RANK },         \
        { 0 },                                              \
    }

static const struct SocialLinkRankGate sSocialLinkGates_EliteFour[] =
{
    { FLAG_IS_CHAMPION, SOCIAL_LINK_MAX_RANK },
    { 0 },
};

#define POKEMON_LINK(_name)                         \
    {                                               \
        .name = COMPOUND_STRING(_name),             \
        .kind = SOCIAL_LINK_KIND_POKEMON,           \
        .type = TYPE_NONE,                          \
        .baseRankCap = 2,                           \
        .gates = sSocialLinkGates_Story,            \
    }

#define GYM_LEADER_LINK(_name, _type, defeatedFlag) \
    {                                               \
        .name = COMPOUND_STRING(_name),             \
        .kind = SOCIAL_LINK_KIND_NPC,               \
        .type = _type,                              \
        .baseRankCap = 3,                           \
        .gates = GYM_LEADER_GATES(defeatedFlag),    \
    }

#define ELITE_FOUR_LINK(_name, _type)               \
    {                                               \
        .name = COMPOUND_STRING(_name),             \
        .kind = SOCIAL_LINK_KIND_NPC,               \
        .type = _type,                              \
        .baseRankCap = 3,                           \
        .gates = sSocialLinkGates_EliteFour,        \
    }

const struct SocialLinkInfo gSocialLinksInfo[NUM_SOCIAL_LINKS] =
{
    [SOCIAL_LINK_STARTER]     = POKEMON_LINK("Partner"),
    [SOCIAL_LINK_EVENT_MON_1] = POKEMON_LINK("Event Pokémon 1"),
    [SOCIAL_LINK_EVENT_MON_2] = POKEMON_LINK("Event Pokémon 2"),
    [SOCIAL_LINK_EVENT_MON_3] = POKEMON_LINK("Event Pokémon 3"),
    [SOCIAL_LINK_EVENT_MON_4] = POKEMON_LINK("Event Pokémon 4"),

    [SOCIAL_LINK_RIVAL] =
    {
        .name = COMPOUND_STRING("Rival"),
        .kind = SOCIAL_LINK_KIND_NPC,
        .type = TYPE_GRASS,
        .baseRankCap = 2,
        .gates = sSocialLinkGates_Story,
    },
    [SOCIAL_LINK_WALLY] =
    {
        .name = COMPOUND_STRING("Wally"),
        .kind = SOCIAL_LINK_KIND_NPC,
        .type = TYPE_FAIRY,
        .baseRankCap = 2,
        .gates = sSocialLinkGates_Story,
    },
    [SOCIAL_LINK_STEVEN] =
    {
        .name = COMPOUND_STRING("Steven"),
        .kind = SOCIAL_LINK_KIND_NPC,
        .type = TYPE_STEEL,
        .baseRankCap = 2,
        .gates = sSocialLinkGates_Story,
    },

    [SOCIAL_LINK_ROXANNE]       = GYM_LEADER_LINK("Roxanne", TYPE_ROCK, FLAG_DEFEATED_RUSTBORO_GYM),
    [SOCIAL_LINK_BRAWLY]        = GYM_LEADER_LINK("Brawly", TYPE_FIGHTING, FLAG_DEFEATED_DEWFORD_GYM),
    [SOCIAL_LINK_WATTSON]       = GYM_LEADER_LINK("Wattson", TYPE_ELECTRIC, FLAG_DEFEATED_MAUVILLE_GYM),
    [SOCIAL_LINK_FLANNERY]      = GYM_LEADER_LINK("Flannery", TYPE_FIRE, FLAG_DEFEATED_LAVARIDGE_GYM),
    [SOCIAL_LINK_NORMAN]        = GYM_LEADER_LINK("Norman", TYPE_NORMAL, FLAG_DEFEATED_PETALBURG_GYM),
    [SOCIAL_LINK_WINONA]        = GYM_LEADER_LINK("Winona", TYPE_FLYING, FLAG_DEFEATED_FORTREE_GYM),
    [SOCIAL_LINK_TATE_AND_LIZA] = GYM_LEADER_LINK("Tate & Liza", TYPE_PSYCHIC, FLAG_DEFEATED_MOSSDEEP_GYM),
    [SOCIAL_LINK_JUAN]          = GYM_LEADER_LINK("Juan", TYPE_WATER, FLAG_DEFEATED_SOOTOPOLIS_GYM),

    [SOCIAL_LINK_SIDNEY] = ELITE_FOUR_LINK("Sidney", TYPE_DARK),
    [SOCIAL_LINK_PHOEBE] = ELITE_FOUR_LINK("Phoebe", TYPE_GHOST),
    [SOCIAL_LINK_GLACIA] = ELITE_FOUR_LINK("Glacia", TYPE_ICE),
    [SOCIAL_LINK_DRAKE]  = ELITE_FOUR_LINK("Drake", TYPE_DRAGON),

    // Granddaughter of the Mt. Pyre orb keepers. A friendship link with no type, so it gives no shiny bonus.
    // Raised by her scenes and by partner battles with her.
    [SOCIAL_LINK_FAE] =
    {
        .name = COMPOUND_STRING("Fae"),
        .kind = SOCIAL_LINK_KIND_NPC,
        .type = TYPE_NONE,
        .baseRankCap = 2,
        .gates = sSocialLinkGates_Story,
    },
};

#undef POKEMON_LINK
#undef GYM_LEADER_LINK
#undef ELITE_FOUR_LINK
#undef GYM_LEADER_GATES
