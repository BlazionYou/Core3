-- Custom Rancor Hammer Loot Item
rancor_hammer = {
	minimumLevel = 0,
	maximumLevel = -1,
	customObjectName = "Rancor Hammer",
	directObjectTemplate = "object/weapon/melee/2h_sword/2h_sword_maul.iff",
	craftingValues = {
		{"mindamage", 400, 700, 0},
		{"maxdamage", 800, 1400, 0},
		{"attackspeed", 10.0, 6.0, 1},
		{"woundchance", 35, 65, 1},
		{"roundsused", 40, 80, 0},
		{"hitpoints", 5000, 10000, 0},
		{"zerorange", 0, 0, 0},
		{"zerorangemod", 190, 290, 0},
		{"midrange", 6, 6, 0},
		{"midrangemod", 32, 132, 0},
		{"maxrange", 18, 18, 0},
		{"maxrangemod", 0, 50, 0},
		{"attackhealthcost", 40, 40, 0},
		{"attackactioncost", 35, 35, 0},
		{"attackmindcost", 50, 50, 0},
	},
	customizationStringNames = {},
	customizationValues = {},
	randomDotChance = 0,
	junkDealerTypeNeeded = JUNKARMS,
	junkMinValue = 10000,
	junkMaxValue = 20000
}

addLootItemTemplate("rancor_hammer", rancor_hammer)
