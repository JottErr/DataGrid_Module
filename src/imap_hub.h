#ifndef DATAGRIDHUB_H
#define DATAGRIDHUB_H

#include "influence_area_data.h"
#include "imap_manager.h"
#include <godot_cpp/classes/object.hpp>

using namespace godot;

class IMapHub : public Object {
	GDCLASS(IMapHub, Object);

private:
	static IMapHub *singleton;
	IMapManager *world_grid_manager;
	TypedArray<InfluenceAreaData> registered_components;

protected:
	static void _bind_methods();

public:
	static IMapHub *get_singleton();

	IMapHub();
	~IMapHub();

	void set_world_grid_manager(IMapManager *p_world_grid_manager);
	IMapManager *get_world_grid_manager() const;
	void add_influence_area_data(Ref<InfluenceAreaData> p_datagrid_component_data_res);
	TypedArray<InfluenceAreaData> get_registered_components_data_res() { return registered_components; }
	void remove_components(const TypedArray<int> &p_indices);
};

#endif // DATAGRIDHUB_H