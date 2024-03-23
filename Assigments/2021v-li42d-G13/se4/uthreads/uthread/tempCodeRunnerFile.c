	// Estamos a tentar fazer um clone para nao afetarmos diretamente a ready_queue
	// ver quantos elementos tem a lista list
	//list_entry_t * n1 = (list_entry_t *) malloc(sizeof (list_entry_t));
	//list_entry_t * n1 = list;

	//duplicate a list
	/*uthread_t *priority_node = NULL;
	int highest_priority = 0;
	uthread_t *extract=NULL;
	int i=0;

	while (!is_list_empty(n1))
	{
		//priority_node = container_of(remove_list_first(n1),uthread_t,entry);
		extract =  container_of(remove_list_first(n1),uthread_t,entry);
		if (extract!=NULL)
		{
			if (extract->priority > highest_priority)
        	{
				priority_node = extract;
            	highest_priority = extract->priority;
				printf("high %d\n",highest_priority);
			}
		}
		i++;
	}*/