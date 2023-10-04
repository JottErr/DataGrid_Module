class_name ManagerVisualizer
extends Node2D


@export var layer: int = 0
@export var datagrids: Dictionary = {}
@export var base_color: Color = Color(1.0, 1.0, 1.0, 1.0)


func _ready() -> void:
	top_level = true
	z_index = -1


func _draw():
	if datagrids.is_empty():
		return
	for datagrid_index in datagrids:
		var datagrid: DataGrid = datagrids[datagrid_index]
		var data := datagrid.get_data()
		var size := datagrid.get_size_in_cells()
		var cell_size := datagrid.get_cell_size()
		var offset: Vector2 = datagrid_index * size * cell_size
		for y in size.y:
			var row := y * size.x
			var y_pos := y * cell_size
			for x in size.x:
				var x_pos := x * cell_size
				var color := base_color
				color.a = data[x + row]
				draw_rect(Rect2(x_pos + offset.x, y_pos + offset.y, cell_size, cell_size), color)


func _on_data_grid_manager_updated(datagrid_collection: Dictionary) -> void:
	datagrids.clear()
	for datagrid_index in datagrid_collection:
		var layerstack: Dictionary = datagrid_collection.get(datagrid_index)
		var datagrid: DataGrid = layerstack.get(layer, null)
		datagrids[datagrid_index] = datagrid 
	queue_redraw()
