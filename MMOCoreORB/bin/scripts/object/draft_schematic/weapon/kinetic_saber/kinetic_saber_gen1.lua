
object_draft_schematic_weapon_kinetic_saber_kinetic_saber_gen1 = object_draft_schematic_weapon_kinetic_saber_shared_kinetic_saber_gen1:new {

   templateType = DRAFTSCHEMATIC,

   customObjectName = "First Generation Lightsaber",

   craftingToolTab = 2048, -- (See DraftSchematicObjectTemplate.h)
   complexity = 16, 
   size = 1, 
   factoryCrateType = "object/factory/factory_crate_weapon.iff",
   
   xpType = "jedi_general", 
   xp = 0, 

   assemblySkill = "jedi_saber_assembly", 
   experimentingSkill = "jedi_saber_experimentation", 
   customizationSkill = "jedi_customization", 
   factoryCrateSize = 0,

   customizationOptions = {},
   customizationStringNames = {},
   customizationDefaults = {},

   ingredientTemplateNames = {"craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n", "craft_weapon_ingredients_n"},
   -- IFF slot order (must match shared_kinetic_saber_genN.iff):
   --   0:emitter_shroud 1:primary_crystal 2:activator 3:handgrip
   --   4:focusing_crystals 5:previous_gen_saber 6:energizers
   -- Type 1 = ComponentSlot (accepts the previous-gen weapon at index 5)
   ingredientTitleNames = {"emitter_shroud", "primary_crystal", "activator", "handgrip", "focusing_crystals", "power_field_insulator", "energizers"},
   ingredientSlotType = {0, 1, 0, 0, 1, 0, 0},
   resourceTypes = {"mineral", "object/tangible/component/weapon/lightsaber/shared_lightsaber_refined_crystal_pack.iff", "metal", "chemical", "object/tangible/component/weapon/lightsaber/shared_lightsaber_refined_crystal_pack.iff", "gas", "metal"},
   resourceQuantities = {15, 1, 14, 17, 1, 20, 14},
   contribution = {100, 100, 100, 100, 100, 100, 100},

   targetTemplate = "object/weapon/melee/sword/kinetic_saber/kinetic_saber_gen1.iff",

   additionalTemplates = {}

}

ObjectTemplates:addTemplate(object_draft_schematic_weapon_kinetic_saber_kinetic_saber_gen1, "object/draft_schematic/weapon/kinetic_saber/kinetic_saber_gen1.iff")
