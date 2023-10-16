#ifndef INFLUENCEAREA_H
#define INFLUENCEAREA_H

#include <godot_cpp/classes/node2d.hpp>
#include "influence_area_data.h"

namespace godot {

class InfluenceAreaData;

class InfluenceArea : public Node2D {
	GDCLASS(InfluenceArea, Node2D)

private:
	Ref<InfluenceAreaData> data_resource;
	float time_since_registration;
	float registration_frequency;
	
protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	InfluenceArea();
	~InfluenceArea();

	void set_data_resource(Ref<InfluenceAreaData> p_data_resource);
	Ref<InfluenceAreaData> get_data_resource();
	void set_layer(int p_layer);
	int get_layer() const;
	void set_radius(float p_radius);
	float get_radius() const;
	void set_time_since_registration(int p_time);
	int get_time_since_registration() const;
	void set_registration_frequency(float p_freq);
	float get_registration_frequency() const;
	void _process(float p_delta);
};
}

#endif // INFLUENCEAREA_H