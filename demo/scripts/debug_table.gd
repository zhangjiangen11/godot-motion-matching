extends Control

@export var motion_matcher : MotionMatcher
@export var title_scene : PackedScene
@export var cell_scene : PackedScene

@onready var table = $Panel/TableContainer

var cells : Array[TableCell] = []

func _ready() -> void:
	for c in table.get_children():
		c.queue_free()
	
	table.columns = 4
	motion_matcher.on_query_result.connect(_on_query_result)
	
func _on_query_result(data : Dictionary):

	for c in cells:
		c.queue_free()
	cells.clear()
	
	table.columns = len(data.keys()) - 1
	
	for key in data.keys():
		if(key == "frame_data"):
			continue
		add_title(key)
		
	for val in data.values():
		if(val is PackedFloat32Array):
			continue
		add_cell(str(val))

func add_title(data : String):
	var cell : TableCell = title_scene.instantiate() as TableCell
	cell.set_data(data)
	table.add_child(cell)
	cells.append(cell)

func add_cell(data : String):
	var cell : TableCell = cell_scene.instantiate() as TableCell
	cell.set_data(data)
	table.add_child(cell)
	cells.append(cell)