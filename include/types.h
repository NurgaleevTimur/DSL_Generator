#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>
#include <map>



/**
 * @brief Структура, описывающая блок
 */
struct block {
	int sid;
	std::string type;
	std::string name;
	std::map<std::string, std::string> params;
	std::vector<int> dep;				// Sid блоков, от которых зависит этот блок
	int degree;
	bool flag;
};




#endif /* TYPES_H */