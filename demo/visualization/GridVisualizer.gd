class_name GridVisualizer
extends Node2D


@export var datagrid: DataGrid = null
@export var base_color: Color = Color(1.0, 1.0, 1.0, 1.0)


func _ready() -> void:
	top_level = true
	z_index = -1


func _draw():
	if datagrid == null:
		return
	var data := datagrid.get_data()
	var size := datagrid.get_size_in_cells()
	var cell_size := datagrid.get_cell_size()
	for y in size.y:
		var row := y * size.x
		var y_pos := y * cell_size
		for x in size.x:
			var x_pos := x * cell_size
			var color := base_color
			color.a = data[x + row]
			draw_rect(Rect2(x_pos, y_pos, cell_size, cell_size), color)


func _on_data_grid_updated(_datagrid: DataGrid) -> void:
	datagrid = _datagrid
	queue_redraw()
