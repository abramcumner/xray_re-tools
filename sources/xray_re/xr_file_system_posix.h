#ifdef __linux__

#pragma once

// Заголовочные файлы проекта.
#include "xr_file_system.h"

// Заголовочные файлы стандартной библиотеки C.
#include <stdio.h>

namespace xray_re {

/// \brief Запись данных в файл на платформе \a POSIX.
/// \warning Для работы требуется поддержка модуля \a filesystem из \a C++17: для GCC - с 9-й версии.
class xr_file_writer_posix: public xr_writer
{
public:
	explicit xr_file_writer_posix(FILE* p_FD);
	virtual	~xr_file_writer_posix();

	/// \brief Запись данных.
	/// \param [in] src - массив данных.
	/// \param [in] src_size - размер массива, в байтах.
	virtual void w_raw(const void* src, size_t src_size) override;

	/// \param [in] pos - позиция, на которую будет установлен файловый курсор.
	virtual void seek(size_t pos) override;

	/// \return текущая позиция файлового курсора.
	virtual size_t tell() override;

private:
	FILE* m_FD;

	/// \note Стандартные конструкторы должны быть удалены, т. к. класс требует для работы верный файловый дескриптор.
	xr_file_writer_posix() = delete;
};

} // end of namespace xray_re

#endif // __linux__
