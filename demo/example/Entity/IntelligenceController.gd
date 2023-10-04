extends Node2D

signal found_target (target_pos: Vector2)

@export var interest_grid : DataGrid
@export var interest_grid_size := Vector2i(31, 31)
@onready var think_cycle_timer: Timer = $ThinkCycleTimer

@onready var target_sprite: Sprite2D = $TargetSprite
@onready var grid_visualizer: Node2D = $GridVisualizer

var cell_size := 10.0
var cell_center := Vector2(cell_size, cell_size) / 2.0

var interest_template := DataGrid.new()


func _ready() -> void:
	var curve := MathCurve.new()
	curve.set_parameters(-1.0, 4, 0.0, 1.0)
	interest_template.set_cell_size(cell_size)
	interest_template.set_size_in_cells(interest_grid_size)
	interest_template.radiate_value_at_position(interest_template.get_center(), interest_template.get_center().x, curve)
	
	think_cycle_timer.start()


func _on_think_cycle_timer_timeout() -> void:
	var flat_curve := MathCurve.new()
	flat_curve.set_parameters(1.0, 0.0, 0.0, 0.0)
	interest_grid = DataGrid.new()
	interest_grid.set_cell_size(cell_size)
	interest_grid.set_size_in_cells(interest_grid_size)
	interest_grid.radiate_value_at_position(interest_grid.get_center(), interest_grid.get_center().x, flat_curve)
	
	#add proximity layer of world datagrid to interest grid
	var world_grid_manager := DataGridHub.get_world_grid_manager()
	if world_grid_manager == null:
		print("no manager")
		return

	world_grid_manager.add_into_datagrid_from_collection(interest_grid, 1, global_position, -1.0)
	#multiply same sized grids
	for y in interest_grid_size.y:
		for x in interest_grid_size.x:
			interest_grid.get_data()[x + y * interest_grid_size.x] *= interest_template.get_data()[x + y * interest_grid_size.x]
	
	var target: Vector2i = interest_grid.get_highest_cell()
	found_target.emit(Vector2(target - interest_grid.get_center()))
	var offset_of_interest_grid := snap_to_cell_center(global_position) - Vector2(interest_grid.get_center()) * cell_size - cell_center
	target_sprite.global_position = get_world_pos_from_cell(Vector2i(), target) + offset_of_interest_grid
	
	var half_grid_size_pixel: Vector2 = interest_grid.get_center() * interest_grid.get_cell_size()
	grid_visualizer.global_position = snap_to_cell_center(global_position - half_grid_size_pixel) - cell_center
	grid_visualizer._on_data_grid_updated(interest_grid)


func snap_to_cell_center(pos : Vector2) -> Vector2:
	return Vector2(pos / cell_size).floor() * cell_size + cell_center


func get_world_pos_from_cell(datagrid_position: Vector2i, cell_position: Vector2i) -> Vector2:
	var manager := DataGridHub.get_world_grid_manager()
	var position = cell_position * cell_size + datagrid_position * manager.datagrid_size * cell_size
	return position
