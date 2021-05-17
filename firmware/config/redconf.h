/*
 * redconf.h
 * 
 * Copyright (C) 2021, SpaceLab.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Reliance Edge configuration definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.30
 * 
 * \date 2021/05/16
 * 
 * \defgroup redconf Reliance Edge Configuration
 * \ingroup config
 * \{
 */

#ifndef REDCONF_H_
#define REDCONF_H_

#define REDCONF_READ_ONLY               0

#define REDCONF_API_POSIX               0

#define REDCONF_API_FSE                 1

#define REDCONF_API_POSIX_FORMAT        1

#define REDCONF_API_POSIX_LINK          1

#define REDCONF_API_POSIX_UNLINK        1

#define REDCONF_API_POSIX_MKDIR         0

#define REDCONF_API_POSIX_RMDIR         0

#define REDCONF_API_POSIX_RENAME        0

#define REDCONF_RENAME_ATOMIC           1

#define REDCONF_API_POSIX_FTRUNCATE     1

#define REDCONF_API_POSIX_READDIR       0

#define REDCONF_API_POSIX_CWD           0

#define REDCONF_API_POSIX_FSTRIM        0

#define REDCONF_NAME_MAX                12U

#define REDCONF_PATH_SEPARATOR          '/'

#define REDCONF_TASK_COUNT              10U

#define REDCONF_HANDLE_COUNT            10U

#define REDCONF_API_FSE_FORMAT          1

#define REDCONF_API_FSE_TRUNCATE        0

#define REDCONF_API_FSE_TRANSMASKGET    0

#define REDCONF_API_FSE_TRANSMASKSET    0

#define REDCONF_OUTPUT                  1

#define REDCONF_ASSERTS                 1

#define REDCONF_BLOCK_SIZE              512U

#define REDCONF_VOLUME_COUNT            1U

#define REDCONF_ENDIAN_BIG              0

#define REDCONF_ALIGNMENT_SIZE          4U

#define REDCONF_CRC_ALGORITHM           CRC_SLICEBY8

#define REDCONF_INODE_BLOCKS            1

#define REDCONF_INODE_TIMESTAMPS        1

#define REDCONF_ATIME                   0

#define REDCONF_DIRECT_POINTERS         4U

#define REDCONF_INDIRECT_POINTERS       32U

#define REDCONF_BUFFER_COUNT            12U

#define RedMemCpyUnchecked              memcpy

#define RedMemMoveUnchecked             memmove

#define RedMemSetUnchecked              memset

#define RedMemCmpUnchecked              memcmp

#define RedStrLenUnchecked              strlen

#define RedStrCmpUnchecked              strcmp

#define RedStrNCmpUnchecked             strncmp

#define RedStrNCpyUnchecked             strncpy

#define REDCONF_TRANSACT_DEFAULT        (( RED_TRANSACT_CREAT | RED_TRANSACT_MKDIR | RED_TRANSACT_RENAME | RED_TRANSACT_LINK | RED_TRANSACT_UNLINK | RED_TRANSACT_FSYNC | RED_TRANSACT_CLOSE | RED_TRANSACT_VOLFULL | RED_TRANSACT_UMOUNT | RED_TRANSACT_SYNC ) & RED_TRANSACT_MASK)

#define REDCONF_IMAP_INLINE             0

#define REDCONF_IMAP_EXTERNAL           1

#define REDCONF_DISCARDS                0

#define REDCONF_IMAGE_BUILDER           0

#define REDCONF_CHECKER                 0

#define RED_CONFIG_UTILITY_VERSION      0x2050000U

#define RED_CONFIG_MINCOMPAT_VER        0x2030000U

#endif /* REDCONF_H_ */

/** \} End of redconf group */
