-- Acklay Boss Loot Group
-- Copy of acklay with boss drop rates

acklay_boss = {
	description = "",
	minimumLevel = 0,
	maximumLevel = 0,
	lootItems = {
		{itemTemplate = "acklay_bones_rare", weight = 7500000},
		{itemTemplate = "acklay_ris_armor_schematic", weight = 2500000}

	}
}

addLootGroupTemplate("acklay_boss", acklay_boss)
