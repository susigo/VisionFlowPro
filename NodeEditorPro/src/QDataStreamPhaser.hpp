#pragma once
#include <QDataStream>
#include <iostream>
#include <QColor>
#include "halconcpp/HalconCpp.h"

template<typename T>
inline void loadFromData(const QString _fileName, std::vector<T>& _obj);

template<typename T>
inline void saveToData(const QString _fileName, const std::vector<T>& _obj);

