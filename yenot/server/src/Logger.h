/**
	@file
	@brief Заголовочный файл модуля логирования
	@author SavaLione
	@date 1 Apr 2018
*/
/**
	@defgroup loggerh Logger.h
	@ingroup yenot
	@{
*/
#ifndef LOGGER_H
#define LOGGER_H

/**
	@brief Основная функция для логирования
	
	Логирование возможно с разными уровнями и с выводом времени
	
	@param [in] level Уровень логирования
	@param [in] text Текст для логирования
*/
void logger(char *level, char *text);

#endif // LOGGER_H
/** @} */