extends Node2D


@onready var grid_visualizer: GridVisualizer = $GridVisualizer


var datagrid: InfluenceMap
var width: int


func _ready() -> void:
	datagrid = grid_visualizer.datagrid
	width = datagrid.get_size_in_cells().x


func _input(event: InputEvent) -> void:
	if Input.is_action_just_pressed("ui_focus_next"):
		var mouse := get_global_mouse_position()
		var mouse_dir := mouse.normalized()
		var cw := Vector2(mouse_dir.y, -mouse_dir.x)
		var ccw := Vector2(mouse_dir.y, mouse_dir.x)
		
		for offset in [Vector2(), cw, ccw, cw*2, ccw*2]:
			var start: Vector2i = Vector2(Vector2() + offset).round()
			var end: Vector2i = Vector2(mouse / 10.0 + offset).round()
		
			var line = get_line_cell_indices(start, end)
			for point in line:
				datagrid.set_cell_value(point, 1.0)
		grid_visualizer._on_data_grid_updated(datagrid)
	

func get_line_cell_indices(start: Vector2, end: Vector2) -> Array[Vector2i]:
	var line: Array[Vector2i] = []
	var steps: float = end.distance_to(start)
	for i in steps:
		var t: float = i / steps
		var point: Vector2i = start * (1.0 - t) + t * end
		line.append(point)
	return line


