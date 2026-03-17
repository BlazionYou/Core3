-- Miniature 0.1 Scale Acklay Mini Boss
acklay_mini_boss = Creature:new {
	objectName = "@mob/creature_names:geonosian_acklay_bunker_boss",
	customName = "Acklay Mini Boss",
	socialGroup = "geonosian_creature",
	mobType = MOB_CARNIVORE,
	faction = "",
	level = 290,
	chanceHit = 0.75,
	damageMin = 1800,
	damageMax = 3200,
	baseXp = 100000,
	baseHAM = 250000,
	baseHAMmax = 300000,
	armor = 2,
	resists = {95,95,-1,95,95,95,-1,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.05,
	ferocity = 95,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	scale = 0.1,
	templates = {"object/mobile/acklay_hue.iff"},
	hues = { 0, 1, 2, 3, 4, 5, 6, 7 },
	lootGroups = {
		{
			groups = {
				{group = "acklay_boss", chance = 10000000}
			},
			lootChance = 10000000
		}
	},

	primaryWeapon = "unarmed",
	secondaryWeapon = "none",
	conversationTemplate = "",
	primaryAttacks = { {"stunattack",""}, {"intimidationattack",""}, {"creatureareakill",""} },
	secondaryAttacks = { }
}

CreatureTemplates:addCreatureTemplate(acklay_mini_boss, "acklay_mini_boss")
