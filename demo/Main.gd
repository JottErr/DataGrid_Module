extends Node2D


@onready var data_grid_manager: DataGridManager = $DataGridManager
@onready var manager_visualizer: Node2D = $ManagerVisualizer


func _ready() -> void:
	DataGridHub.set_world_grid_manager(data_grid_manager)
	for wall in $Map.get_children():
		wall.register_in_manager(data_grid_manager)
	data_grid_manager.initialize_templates(1, 21, 1)


func _input(event: InputEvent) -> void:
	if event is InputEventKey:
		if event.pressed and event.keycode in range(48, 58):
			manager_visualizer.layer = event.keycode - 48


func test_performance() -> void:
	var testcurve := MathCurve.new()
	var data1 := InfluenceMap.new()
	data1.set_size(Vector2i(9, 9))
	data1.radiate_value_at_position(Vector2i(4, 4), 4, testcurve, 1.0)
	var data2 := InfluenceMap.new()
	data2.set_size(Vector2i(600, 600))
	for n in 10:
		var start = Time.get_ticks_usec()
		for i in 600:
			for j in 600:
				data2.add_grid_centered_at_pos(data1, Vector2i(i, j), 1.0)
		print(Time.get_ticks_usec() - start)


func test_manager_performance() -> void:
	var manager := DataGridManager.new()
	manager.set_world_size(Vector2(6000, 6000))
	manager.set_cell_size(10)
	manager.set_datagrid_count(Vector2i(6, 6))
	
	var testcurve := MathCurve.new()
	var data1 := InfluenceMap.new()
	data1.set_size(Vector2i(9, 9))
	data1.radiate_value_at_position(Vector2i(4, 4), 4, testcurve, 1.0)

	for n in 10:
		var start = Time.get_ticks_usec()
		for i in 600:
			for j in 600:
				manager.add_datagrid_centered_to_collection(data1, 1, Vector2i(i, j), 1.0)
		print(Time.get_ticks_usec() - start)
