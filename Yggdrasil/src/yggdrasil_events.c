
//install event
void InstallEvent(uchar type, Shell_Object * obj, void (* handle)(struct Shell_Object *sender))
{
	Heap_Manager * heap = obj->heap; 
	Event_Handler * event = m_heap_alloc(heap, sizeof(Event_Handler));
	Event_Handler * temp;
	event->type = type;
	event->next = NULL;
	event->handle = handle;
	if(obj->events == NULL)
	{
		obj->events = event;
	} else {
		temp = obj->events;
		while(temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = event;
	}
}

void DeleteEvent(Shell_Object * obj, void (* handle)(struct Shell_Object *sender))
{
	Heap_Manager * heap = obj->heap; 
	Event_Handler * temp;
	Event_Handler * prev;
	if(obj->events == NULL) return;
	else {
		temp = obj->events;
		if(temp->handle == handle) {
			obj->events = temp->next;
			m_heap_free(heap, temp);
		} else {
			while(temp->next != NULL) {
				if(temp->handle == handle) {
					prev->next = temp->next;
					m_heap_free(heap, temp);
				}
				prev = temp;
				temp = temp->next;
			}
		}
	}
}