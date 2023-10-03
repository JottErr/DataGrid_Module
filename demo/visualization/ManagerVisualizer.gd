class_name ManagerVisualizer
extends GridVisualizer


@export var datagrid_position: Vector2i = Vector2i(0, 0)
@export var layer: int = 0


func _on_data_grid_manager_updated(datagrid_collection: Dictionary) -> void:
	var layerstack: Dictionary = datagrid_collection.get(datagrid_position, {})
	var _datagrid: DataGrid = layerstack.get(layer, null)
	_on_data_grid_updated(_datagrid)
