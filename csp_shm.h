/**
* Copyright (C) - All Rights Reserved.
*
* File Name: csp_shm.h
* Summary  : Define the crowded sourcing platform shared memory utils.
*
* Version  : 1.0
* Author   : weixun
* Date     : 2019.10.27
* std I/F  : shmget (create or get shared memory chunck)
*            shmat  (Mapping the Shared memory area object to the address space of the calling process)
*            shmdt  (disconnect shared memory)
*            shmctl (shared memory manage)
* Remark   : multiple process read shared memory currently is not supported
**/

#ifndef CSP_SHM_H
#define CSP_SHM_H

#include "define.h"
#include "cmn_hdr.h"
#include "csp_mutex.h"
#include "csp_thread.h"

#define  SHM_BUF_SIZE_MAX       (10*1024)  // 10KB
#define  SHM_BUF_SIZE_MIN       (2*1024)   //  2KB
#define  SHM_BUF_SIZE_DEFAULT   (5*1024)   //  5KB

namespace csp {
    namespace utils {
        /************************************************
         *    Class shm                                 *
         ************************************************/
        class shm : public csp_noncopyable
        {
        public:
            /************************************************
             *    struct shm_chunk                          *
             ************************************************/
            typedef struct shm_chunk
            {
                int     _rw_flag;                           // 0: writeable  1: readable (非0：表示可读, 0：表示可写)
                char    _shm_buf[SHM_BUF_SIZE_DEFAULT];     // shm buf
                int     _shm_data_len;                      // shm data length

                shm_chunk()
                {
                    _rw_flag  = -1;
                    memset(_shm_buf, 0, SHM_BUF_SIZE_DEFAULT);
                    _shm_data_len = 0;
                }

                shm_chunk(char* buf, int shm_data_len)
                {
                    _rw_flag  = -1;
                    memcpy(_shm_buf, buf, shm_data_len);
                    _shm_data_len = shm_data_len;
                }
            }shm_chunk;

        public:
            /**
             * @brief constructor default
             */
            shm();

            /**
             * @brief destructor default
             */
            ~shm();

        /************************************************
         *    External InterFace                        *
         ************************************************/
        public:
            /**
             * @brief get shm object
             * @return shm id
             */
            int   get_shm();


            /**
             * @brief create shm object with specified key
             * @param key assign to shared memory name
             * @return shm id
             */
            int   get_shm(key_t key);


            /**
             * @brief attach shm segment
             */
            void* attach_shm();


            /**
             * @brief create shm object
             */
            void create_shm();


            /**
             * @brief create shm object
             * @param key assign to shared memory name
             */
            void create_shm(key_t key);


            /**
             * @brief read object from shm
             */
            void* read_shm();


            /**
             * @brief write object(char*) to shm
             */
            void  write_shm(char* _data);


            /**
             * @brief detach shm from process
             * @return -1: failed
             *          0: succeed
             */
            int   detach_shm();


            /**
             * @brief delete shm
             */
            void  del_shm();


        /************************************************
         *    Internal InterFace                        *
         ************************************************/
        private:
            /**
             * @brief get shm size
             * @param shm_size setting shared memory size
             */
            void set_shm_size(int32_t shm_size);


            /**
             * @brief get shm chunk
             * @return shm chunk ptr
             */
            shm_chunk* get_shm_chunk();


            /**
             * @brief get shm id
             * @return shm id
             */
            int get_shmid();


            /**
             * @brief get shm addr
             * @return shm addr
             */
            void* get_shm_addr();

        private:
            int              _shmid;     // shared memory id
            int32_t          _shm_size;  // shared memory init size
            void*            _shm_addr;  // shared memory address
            bool             _flag;      // shared memory create flag
            shm_chunk*       _shm_chunk; // shared memory chunck
            csp_utils_mutex  _shm_mutex; // shared memory mutex lock
        };
    }
}

typedef csp::utils::shm             csp_utils_shm;
typedef csp::utils::shm::shm_chunk  csp_utils_shm_chunk;


#endif // CSP_SHM_H
