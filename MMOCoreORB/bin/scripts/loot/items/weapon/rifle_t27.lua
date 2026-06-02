-- Custom Rancor T21 Rifle Loot Item
rifle_t27 = {
	minimumLevel = 0,
	maximumLevel = -1,
	customObjectName = "Rancor T27",
	directObjectTemplate = "object/weapon/ranged/rifle/rifle_t21.iff",
	craftingValues = {
		{"mindamage", 1580, 2450, 0},
		{"maxdamage", 2420, 3750, 0},
		{"attackspeed", 10.0, 2.0, 1},
		{"woundchance", 35, 95, 1},
		{"roundsused", 40, 80, 0},
		{"hitpoints", 10000, 20000, 0},
		{"zerorange", 0, 0, 0},
		{"zerorangemod", -90, 90, 0},
		{"midrange", 84, 84, 0},
		{"midrangemod", 132, 320, 0},
		{"maxrange", 264, 264, 0},
		{"maxrangemod", 150, 150, 0},
		{"attackhealthcost", 40, 40, 0},
		{"attackactioncost", 35, 35, 0},
		{"attackmindcost", 50, 50, 0},
	},
	customizationStringNames = {},
	customizationValues = {},
	randomDotChance = 0,
	junkDealerTypeNeeded = JUNKARMS,
	junkMinValue = 100000,
	junkMaxValue = 200000
}

addLootItemTemplate("rifle_t27", rifle_t27)