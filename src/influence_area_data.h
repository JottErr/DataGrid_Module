#ifndef INFLUENCEAREADATA_H
#define INFLUENCEAREADATA_H

#include "influence_area.h"

namespace godot {

class InfluenceArea;

class InfluenceAreaData : public Resource {
	GDCLASS(InfluenceAreaData, Resource);

private:
	InfluenceArea *area_node;
	
	int influence_type;
	int layer;
	float radius;
	bool registered;
	Vector2 registered_position;
	int registered_layer;
	float registered_radius;
	bool registerable;

protected:
	static void _bind_methods();

public:
	InfluenceAreaData();
	~InfluenceAreaData();

	void set_area_node(InfluenceArea *p_area_node);
	InfluenceArea *get_area_node() const;
	bool area_node_is_valid() const;
	Vector2 get_global_position() const;
	void set_influence_type(int p_influence_type);
	int get_influence_type() const;
	void set_layer(int p_layer);
	int get_layer() const;
	void set_radius(float p_radius);
	float get_radius() const;
	void set_registered(bool p_registered);
	bool is_registered() const;
	void set_registered_position(const Vector2 &p_position);
	const Vector2 &get_registered_position() const;
	void set_registered_layer(int p_layer);
	int get_registered_layer() const;
	void set_registered_radius(float p_radius);
	float get_registered_radius() const;
	void on_registered(const Vector2 &p_position, int p_layer, float p_radius);
	void set_registerable(bool p_registerable);
	bool is_registerable() const;
};
}

#endif // INFLUENCEAREADATA_H