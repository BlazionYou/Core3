-- Miniature 0.1 Scale Kryat Dragon Ancient Boss
krayt_mini_boss = Creature:new {
	objectName = "@mob/creature_names:krayt_dragon_ancient",
	customName = "Kryat Mini Boss",
	socialGroup = "krayt",
	faction = "",
	mobType = MOB_CARNIVORE,
	level = 290,
	chanceHit = 0.75,
	damageMin = 1800,
	damageMax = 3200,
	baseXp = 100000,
	baseHAM = 250000,
	baseHAMmax = 300000,
	armor = 2,
	resists = {95,95,-1,95,95,95,-1,-1,-1},
	meatType = "meat_carnivore",
	meatAmount = 2950,
	hideType = "hide_bristley",
	hideAmount = 2877,
	boneType = "bone_mammal",
	boneAmount = 2777,
	milk = 0,
	tamingChance = 0.05,
	ferocity = 95,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	scale = 0.05,
	templates = {"object/mobile/krayt_dragon_hue.iff"},
	hues = { 16, 17, 18, 19, 20, 21, 22, 23 },
	lootGroups = {
		{
			groups = {
				{group = "krayt_tissue_rare", chance = 5000000},
				{group = "krayt_pearls", chance = 5000000},
			},
			lootChance = 8000000
		}
	},

	primaryWeapon = "unarmed",
	secondaryWeapon = "none",
	conversationTemplate = "",
	primaryAttacks = { {"stunattack",""}, {"intimidationattack",""}, {"creatureareakill",""} },
	secondaryAttacks = { }
}

CreatureTemplates:addCreatureTemplate(krayt_mini_boss, "krayt_mini_boss")
