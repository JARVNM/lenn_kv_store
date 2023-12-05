lenn_kv_store:lenn_kv_store.c lenn_kv_array.c lenn_reactor_entry.c
	gcc -o lenn_kv_store lenn_kv_store.c lenn_kv_array.c lenn_reactor_entry.c
clean:
	rm lenn_kv_store