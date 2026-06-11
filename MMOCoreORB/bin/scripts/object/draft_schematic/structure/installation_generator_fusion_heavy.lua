object_draft_schematic_structure_installation_generator_fusion_heavy = object_draft_schematic_structure_shared_installation_generator_fusion:new {

	templateType = DRAFTSCHEMATIC,

	customObjectName = "Deed for: Heavy Fusion Power Generator",

	craftingToolTab = 1024, -- (See DraftSchematicObjectTemplate.h)
	complexity = 35,
	size = 12,
	factoryCrateSize = 1,
	factoryCrateType = "object/factory/factory_crate_installation.iff",
   
	xpType = "crafting_structure_general",
	xp = 39000,

	assemblySkill = "structure_assembly",
	experimentingSkill = "structure_experimentation",
	customizationSkill = "structure_customization",

	customizationOptions = {},
	customizationStringNames = {},
	customizationDefaults = {},

	ingredientTemplateNames = {"craft_structure_ingredients_n", "craft_structure_ingredients_n", "craft_structure_ingredients_n", "craft_structure_ingredients_n", "craft_structure_ingredients_n", "craft_structure_ingredients_n", "craft_structure_ingredients_n", "craft_structure_ingredients_n", "craft_structure_ingredients_n", "craft_structure_ingredients_n"},
	ingredientTitleNames = {"load_bearing_frame", "body_shell_structure", "ore_extractor_mill", "structure_foundation", "pile_containment_shielding", "pile_containment_unit", "pile_containment_coolant", "matched_turbine_cluster", "capacitance_storage_compartments", "cell_manufacturing_mechanism"},
	ingredientSlotType = {0, 0, 1, 0, 0, 2, 0, 1, 1, 1},
	resourceTypes = {"steel", "metal", "object/tangible/component/structure/shared_ore_mining_unit.iff", "metal", "ore", "object/tangible/component/structure/shared_wall_module.iff", "chemical", "object/tangible/component/structure/shared_generator_turbine.iff", "object/tangible/component/structure/shared_structure_small_storage_section.iff", "object/tangible/component/structure/shared_manufacturing_mechanism.iff"},
	resourceQuantities = {3000, 7000, 1, 2500, 2500, 5, 3000, 3, 4, 2},
	contribution = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100},

	targetTemplate = "object/tangible/deed/generator_deed/generator_fusion_heavy_deed.iff",

	additionalTemplates = {}
}
ObjectTemplates:addTemplate(object_draft_schematic_structure_installation_generator_fusion_heavy, "object/draft_schematic/structure/installation_generator_fusion_heavy.iff")
