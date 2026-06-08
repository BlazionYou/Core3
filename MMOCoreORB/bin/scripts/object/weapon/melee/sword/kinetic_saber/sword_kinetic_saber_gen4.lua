
object_weapon_melee_sword_kinetic_saber_sword_kinetic_saber_gen4 = object_weapon_melee_sword_kinetic_saber_shared_sword_kinetic_saber_gen4:new {
	playerRaces = {
		"object/creature/player/bothan_male.iff",
		"object/creature/player/bothan_female.iff",
		"object/creature/player/human_male.iff",
		"object/creature/player/human_female.iff",
		"object/creature/player/ithorian_male.iff",
		"object/creature/player/ithorian_female.iff",
		"object/creature/player/moncal_male.iff",
		"object/creature/player/moncal_female.iff",
		"object/creature/player/rodian_male.iff",
		"object/creature/player/rodian_female.iff",
		"object/creature/player/sullustan_male.iff",
		"object/creature/player/sullustan_female.iff",
		"object/creature/player/trandoshan_male.iff",
		"object/creature/player/trandoshan_female.iff",
		"object/creature/player/twilek_male.iff",
		"object/creature/player/twilek_female.iff",
		"object/creature/player/wookiee_male.iff",
		"object/creature/player/wookiee_female.iff",
		"object/creature/player/zabrak_male.iff",
		"object/creature/player/zabrak_female.iff"
	},

	attackType = MELEEATTACK,
	damageType = KINETIC,
	armorPiercing = MEDIUM,

	xpType = "crafting_weapons_general",

	certificationsRequired = {},
	creatureAccuracyModifiers = { "onehandlightsaber_accuracy" },
	defenderDefenseModifiers = { "melee_defense" },
	defenderSecondaryDefenseModifiers = { "saber_block" },
	speedModifiers = { "onehandlightsaber_speed" },
	damageModifiers = { },

	healthAttackCost = 20,
	actionAttackCost = 35,
	mindAttackCost = 40,
	forceCost = 40,

	pointBlankRange = 0,
	pointBlankAccuracy = 20,

	idealRange = 3,
	idealAccuracy = 15,

	maxRange = 5,
	maxRangeAccuracy = 5,

	attackSpeed = 4.5,

	woundsRatio = 37,

	defenderToughnessModifiers = { "lightsaber_toughness" },

	noTrade = 1,

	childObjects = {
		{templateFile = "object/tangible/inventory/lightsaber_inventory_4.iff", x = 0, z = 0, y = 0, ox = 0, oy = 0, oz = 0, ow = 0, cellid = -1, containmentType = 4}
	},

	numberExperimentalProperties = {1, 1, 2, 2, 2, 2, 2, 1, 1, 1},
	experimentalProperties = {"XX", "XX", "CD", "OQ", "CD", "OQ", "CD", "OQ", "SR", "UT", "CD", "OQ", "OQ", "OQ", "OQ"},
	experimentalWeights = {1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1},
	experimentalGroupTitles = {"null", "null", "expDamage", "expDamage", "expDamage", "expDamage", "expEffeciency", "expEffeciency", "expEffeciency", "expEffeciency"},
	experimentalSubGroupTitles = {"null", "null", "mindamage", "maxdamage", "attackspeed", "woundchance", "forcecost", "attackhealthcost", "attackactioncost", "attackmindcost"},
	experimentalMin = {0, 0, 140, 230, 4.5, 25, 55, 45, 60, 95},
	experimentalMax = {0, 0, 160, 270, 4.2, 50, 48, 40, 45, 65},
	experimentalPrecision = {0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
	experimentalCombineType = {0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
}

ObjectTemplates:addTemplate(object_weapon_melee_sword_kinetic_saber_sword_kinetic_saber_gen4, "object/weapon/melee/sword/kinetic_saber/shared_kinetic_saber_gen4.iff")
