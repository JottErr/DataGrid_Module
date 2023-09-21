#include "register_types.h"

#include "math_curve.h"
#include "data_grid.h"
#include "data_grid_component.h"
#include "data_grid_manager.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_data_grid_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    ClassDB::register_class<MathCurve>();
    ClassDB::register_class<DataGrid>();
    ClassDB::register_class<DataGridTemplate>();
    ClassDB::register_class<DataGridComponent>();
    ClassDB::register_class<DataGridManager>();
}

void uninitialize_data_grid_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
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