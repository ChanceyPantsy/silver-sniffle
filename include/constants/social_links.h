#ifndef GUARD_CONSTANTS_SOCIAL_LINKS_H
#define GUARD_CONSTANTS_SOCIAL_LINKS_H

// Social link IDs. These are plain defines (not an enum) so that event scripts can use them.
// Each ID needs an entry in gSocialLinksInfo (src/data/social_links.h).

// Pokémon links. These start unbound and are tied to one specific Pokémon
// (by personality + OT ID) with the sociallink_bindpartymon / sociallink_bindspecies script commands.
// Their type is the bound Pokémon's primary type at the moment it was bound.
#define SOCIAL_LINK_STARTER          0
#define SOCIAL_LINK_EVENT_MON_1      1
#define SOCIAL_LINK_EVENT_MON_2      2
#define SOCIAL_LINK_EVENT_MON_3      3
#define SOCIAL_LINK_EVENT_MON_4      4
#define NUM_POKEMON_SOCIAL_LINKS     5 // Pokémon links must be the first IDs.

// NPC links. These are raised from event scripts with sociallink_addpoints.
#define SOCIAL_LINK_RIVAL            5
#define SOCIAL_LINK_WALLY            6
#define SOCIAL_LINK_STEVEN           7
#define SOCIAL_LINK_ROXANNE          8
#define SOCIAL_LINK_BRAWLY           9
#define SOCIAL_LINK_WATTSON         10
#define SOCIAL_LINK_FLANNERY        11
#define SOCIAL_LINK_NORMAN          12
#define SOCIAL_LINK_WINONA          13
#define SOCIAL_LINK_TATE_AND_LIZA   14
#define SOCIAL_LINK_JUAN            15
#define SOCIAL_LINK_SIDNEY          16
#define SOCIAL_LINK_PHOEBE          17
#define SOCIAL_LINK_GLACIA          18
#define SOCIAL_LINK_DRAKE           19

#define NUM_SOCIAL_LINKS            20

#define SOCIAL_LINK_KIND_NPC         0
#define SOCIAL_LINK_KIND_POKEMON     1

#define SOCIAL_LINK_MAX_RANK        10

// Return values of sociallink_bindpartymon / sociallink_bindspecies (written to VAR_RESULT).
#define SOCIAL_LINK_BIND_FAILED      0
#define SOCIAL_LINK_BIND_OK          1

#endif // GUARD_CONSTANTS_SOCIAL_LINKS_H
