shield_generator_personal_imperial = {
	minimumLevel = 0,
	maximumLevel = -1,
	customObjectName = "Imperial Personal Shield Generator",
	directObjectTemplate = "object/tangible/wearables/armor/imperial_shield_generator.iff",
	craftingValues = {
		{"armor_rating", 1, 1, 0},
		{"armor_effectiveness", 20, 30, 10},
		{"armor_integrity", 30000, 55000, 0},
		{"armor_special_type", 0, 0, 0},
		{"armor_special_effectiveness", 30, 50, 10},
		{"armor_special_integrity", 750, 1500, 0},
	},
	skillMods = {},
	customizationStringNames = {},
	customizationValues = {},

	junkDealerTypeNeeded = JUNKGENERIC,
	junkMinValue = 10000,
	junkMaxValue = 20000
}

addLootItemTemplate("shield_generator_personal_imperial", shield_generator_personal_imperial)
