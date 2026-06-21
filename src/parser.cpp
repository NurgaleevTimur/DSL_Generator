#include <algorithm>
#include "types.h"
#include "parser.h"


/**
 * @brief Добавляет зависимость от src к блоку с SID = dst
 * @param blocks Вектор блоков
 * @param src SID источника
 * @param dst SID приёмника
 * @param sid_to_index Ассоциативный массив SID -> индекс
 * @note Для блоков типа UnitDelay степень не увеличивается
 */
static void __feel_dep ( std::vector<block>& blocks, int src, int dst, std::unordered_map<int, int>& sid_to_index )
{
	block& b  = blocks [ sid_to_index [ dst ] ];
	b.dep.push_back ( src );
	if ( b.type != "UnitDelay" ) {
		b.degree++;
	}
}




/**
 * @brief Извлекает SID из строки вида "123#out:1"
 * @param p XML-узел <P>, содержащий строку с SID
 * @return Числовой SID
 */
static int __get_sid ( const pugi::xml_node p )
{
	std::string str = p.child_value();
	size_t pos = str.find ( '#' );

	str = str.substr ( 0, pos );
	return stoi ( str );
}



/**
 * @brief Парсит все блоки из XML-узла <System>
 * @param system Узел <System> XML-документа
 * @param blocks Вектор, в который будут добавлены все блоки
 * @param sid_to_index Ассоциативный массив для быстрого поиска индекса блока по его SID
 */
void parse_blocks ( const pugi::xml_node& t, std::vector<block>& blocks, std::unordered_map<int, int>& sid_to_index )
{
	for ( pugi::xml_node block_node : t.children( "Block" ) ) {
		block b = { b.degree = 0 };
		int index = blocks.size();

		b.sid = block_node.attribute( "SID" ).as_int();
		b.type = block_node.attribute( "BlockType" ).value();
		b.name = block_node.attribute( "Name" ).value();

		b.name.erase( std::remove( b.name.begin(), b.name.end(), ' '), b.name.end() );

		// Парсим все <P>
		for ( pugi::xml_node p : block_node.children ( "P" ) ) {
			std::string key = p.attribute( "Name" ).value();
			std::string value = p.child_value();
			b.params[key] = value;
		}
		
		sid_to_index [ b.sid ] = index;
		blocks.push_back( b );
	}
}




/**
 * @brief Парсит все линии из XML-узла <System> и заполняет зависимости блоков
 * @param system Узел <System> XML-документа
 * @param blocks Вектор блоков
 * @param sid_to_index Ассоциативный массив SID -> индекс для быстрого доступа
 */
void parse_lines ( const pugi::xml_node& t,  std::vector<block>& blocks, std::unordered_map<int, int>& sid_to_index )
{
	for ( pugi::xml_node line_node : t.children ( "Line" ) ) {
		int src;

		for ( pugi::xml_node p : line_node.children ( "P" ) ) {
			std::string s = std::string ( p.attribute ( "Name" ).value() );

			if ( s == "Src" ) {
				src = __get_sid ( p );

			} else if ( s == "Dst" ) {
				__feel_dep ( blocks, src, __get_sid ( p ), sid_to_index );

				break;
			}
		}

		for ( pugi::xml_node branch : line_node.children ( "Branch" ) ) {
			for ( pugi::xml_node p : branch.children ( "P" ) ) {
				if ( std::string ( p.attribute( "Name" ).value() ) == "Dst" ) {
					__feel_dep ( blocks, src, __get_sid ( p ), sid_to_index );

					break;
				}
			}
		}
	}
}

