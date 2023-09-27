extends Sprite2D


@export var datagrid_position: Vector2i = Vector2i(0, 0)
@export var layer : int
@onready var data_grid_manager: DataGridManager = $DataGridManager


func _ready() -> void:
	material.set_shader_parameter("grid_size", data_grid_manager.get_datagrid_size())


func _on_data_grid_manager_datagrid_size_changed(datagrid_size: Vector2i) -> void:
	material.set_shader_parameter("grid_size", datagrid_size)


func _on_data_grid_manager_updated(datagrid_collection: Object) -> void:
	datagrid_collection = datagrid_collection as DataGridCollection
	var datagrid: DataGrid = datagrid_collection.get_layer_at_pos(datagrid_position, layer)
	if datagrid == null:
		print("nothing to show")
		return
	var data := PackedFloat32Array(datagrid.get_data())
	material.set_shader_parameter("data_grid", data)
