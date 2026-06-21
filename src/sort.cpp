#include <iostream>
#include <queue>
#include "sort.h"



/**
 * @brief Рекурсивно добавляет SID блока в очередь, предварительно обработав его зависимости
 * @param blocks Вектор блоков
 * @param b Текущий блок (передаётся по ссылке для изменения)
 * @param order Вектор порядка
 * @param sid_to_index Ассоциативный массив SID -> индекс
 */
static void __push_block ( std::vector<block>& blocks, block& b, std::vector<int>& order, std::unordered_map<int, int>& sid_to_index )
{
	if ( b.flag == 0 ) {
		if ( b.degree != 0 ) {
			for ( int sid : b.dep ) {
				__push_block ( blocks, blocks [ sid_to_index [ sid ] ], order, sid_to_index );
				b.degree--;
			}
			order.push_back ( b.sid );
			b.flag = 1;

		} else {
			order.push_back ( b.sid );
			b.flag = 1;
		}
	}
}




/**
 * @brief Выполняет топологическую сортировку блоков (рекурсивный алгоритм)
 * @param blocks Вектор блоков (уменьшается degree, устанавливается flag)
 * @param order Вектор, в который будут записаны SID блоков в порядке вычисления
 * @param sid_to_index Ассоциативный массив SID -> индекс
 */
void top_sort ( std::vector<block> blocks, std::vector<int>& queue, std::unordered_map<int, int>& sid_to_index )
{
	for ( size_t i = 0; i < blocks.size(); i++ ) {
		__push_block ( blocks, blocks[i], queue, sid_to_index );
	}
}


/**
 * @brief Выполняет топологическую сортировку блоков (алгоритм Кана)
 * @param blocks Вектор блоков (уменьшается degree)
 * @param order Вектор, в который будут записаны SID блоков в порядке вычисления
 * @param sid_to_index Ассоциативный массив SID -> индекс
 */
void top_sort_kahn ( std::vector<block> blocks, std::vector<int>& order, std::unordered_map<int, int>& sid_to_index )
{
	// Построить список потребителей
	std::vector<std::vector<int>> consumers ( blocks.size() );

	for ( size_t i = 0; i < blocks.size(); i++ ) {
		for ( int src_sid : blocks[ i ].dep ) {
			int src_idx = sid_to_index [ src_sid ];
			consumers[ src_idx ].push_back ( i );
		}
	}

	// Инициализировать очередь
	std::queue<int> q;
	for ( size_t i = 0; i < blocks.size(); i++ ) {
		if ( blocks[ i ].type != "Outport" ) {
			if ( blocks[ i ].degree == 0 ) q.push( i );
		}
	}


	while ( !q.empty() ) {
		int idx = q.front();
		q.pop();
		order.push_back( blocks[ idx ].sid );

		for ( int consumer : consumers [ idx ] ) {
			if ( blocks[ consumer ].type != "Outport" ) {
				blocks[ consumer ].degree--;
				if ( blocks[ consumer ].degree == 0 ) q.push ( consumer );
			}
		}
	}
}
