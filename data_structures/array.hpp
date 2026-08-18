#pragma once
#include <cstdint>

namespace array {
	template <typename T>
	struct Dynamic {
		T *data = nullptr;
		size_t size;
		size_t capacity;
	};
  	
	// Funciones auxiliares internas
	template <typename T>
	size_t _find_pow_2(size_t size);
	template <typename T>
  	void _move(Dynamic<T> *source, T *sink);
  	
	// Funciones principales
	template <typename T>
	Dynamic<T> build(size_t size);
	template <typename T>
	inline Dynamic<T> reserve(size_t capacity);
	/*
	template <typename T>
	inline T get(Dynamic<T> *array, size_t index);
	template <typename T>
	inline void set(Dynamic<T> *array, T value, size_t index);
	*/
	template <typename T>
	void push_back(Dynamic<T> *array, T value);
  	template <typename T>
	void pop(Dynamic<T> *array, T *pointer);
	
	size_t _find_pow_2(size_t size) {
		// Encuentra la potencia de 2 mayor o igual que size
		size--;
		size |= size >> 1;
		size |= size >> 2;
		size |= size >> 4;
		size |= size >> 8;
		#if INTPTR_MAX == INT32_MAX
		size |= size >> 16;
		#elif INTPTR_MAX == INT64_MAX
		size |= size >> 16;
		size |= size >> 32;
		#endif
		size++;
		return size;
	}


	void _move(Dynamic<T> *source, T *sink) {
		T *pSink = sink;
		for (T *p = source->data; p < &(source->data[source->size]); p++) {
			*pSink = *p;
			pSink++;
		}

		delete [] source->data;
		source->data = sink;
	
	}

	template <typename T>
	inline Dynamic<T> build(size_t size) {
		size_t capacity = _find_pow_2(size);
		return Dynamic dynamicArray = {
			new T[capacity],
			size,
			capacity
		};
	}
	
	template <typename T>	
	inline Dynamic<T> reserve(size_t capacity) {
		size_t capacity = find_pow_2(capacity);
		return Dynamic<T> array = {
			new T[capacity],
			0,
			capacity,
		}
	}
	
	void reserve(Dynamic<T> *array) {
		size_t capacity = array->capacity << 1ULL;
		T *sink = new T[capacity];
		_move(array, sink);
	}

	template<typename T>
	void reserve(Dynamic<T> *array, size_t capacity) {
		size_t capacity = find_pow_2(capacity);
		T *sink = new T[capacity];
		_move(array, sink);	
	}
	
	void shrink(Dynamic<T> *array) {
		// Si 1/4 de capacity es mayor al size reducimos capacity a la mitad
		if (array->capacity >> 2ULL > array->size) {
			size_t capacity = array->capacity >> 1ULL;
			T *sink = new T[capacity];
			_move(array, sink);	
		}
	}

	/*
	template <typename T>
	inline T get(Dynamic<T> *array, size_t index) {
		return (*array).data[index];
	}

	template <typename T>
	inline void set(Dynamic<T> *array, size_t index, T value) {
		(*array).data[index] = value;
	}
	*/

	template <typename T>
	void push_back(Dynamic<T> *array, T value) {
		// Si el arreglo esta lleno creo uno nuevo con el doble de capacidad
		if (array->size == array->capacity) reserve(array);
		array->data[size] = value;
		array->size++;
	}

	template <typename T>
	void pop(Dynamic<T> *array, T *pointer) {
		for (T *p = pointer, p < &(array->data[array->size]) - 1; p++) {
			*(p) = *(p+1);
		}
		array->size--;
		shrink(array);
	}
}
