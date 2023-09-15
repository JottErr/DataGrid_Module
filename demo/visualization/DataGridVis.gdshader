shader_type canvas_item;

uniform vec2 grid_size;
uniform float data_grid[4080];

void fragment() {
	vec2 cell_coordinates = floor(UV * grid_size);
	int index = int(cell_coordinates.x + cell_coordinates.y * float(grid_size.x));
	float value = data_grid[index];
	vec4 color = vec4(1.0, 1.0, 1.0, value);
	COLOR = color;
}
