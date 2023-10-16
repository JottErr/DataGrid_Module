#include "register_types.h"

#include "math_curve.h"
#include "influence_map.h"
#include "influence_area.h"
#include "influence_area_data.h"
#include "imap_manager.h"
#include "data_grid_hub.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

static DataGridHub *_datagrid_hub;

void initialize_data_grid_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<MathCurve>();
	ClassDB::register_class<InfluenceMap>();
	ClassDB::register_class<InfluenceAreaData>();
	ClassDB::register_class<InfluenceArea>();
	ClassDB::register_class<IMapManager>();
	ClassDB::register_class<DataGridHub>();

	_datagrid_hub = memnew(DataGridHub);
	Engine::get_singleton()->register_singleton("DataGridHub", DataGridHub::get_singleton());
}

void uninitialize_data_grid_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	Engine::get_singleton()->unregister_singleton("DataGridHub");
	//memdelete(_datagrid_hub);
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT data_grid_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_data_grid_module);
	init_obj.register_terminator(uninitialize_data_grid_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}