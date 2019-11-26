#include "csp_shm.h"

namespace csp {
    namespace utils {

        shm::shm() : _shmid(-1), _shm_size(sizeof(csp_utils_shm_chunk)), _shm_addr(NULL), _flag(false) {}

        //shm::shm(int32_t shm_size) : _shmid(-1), _shm_size(shm_size), _shm_addr(NULL), _flag(false) {}

        shm::~shm() { _shm_mutex.destroy(); }

        /**
         * @brief  get shared memory use key IPC_PRIVATE
         * @param  null
         * @return shm id
         */
        int shm::get_shm()
        {
            _shmid = shmget((key_t)IPC_PRIVATE, _shm_size, IPC_CREAT | 0666);
            if(_shmid < 0)
            {
                /* log_output [shmget failed] */
                std::cout << "[csp::utils::shm::get_shm] failed! " << std::endl;
                return -1;
            }
            else
            {
                /* log_output */
                /* shm create succeed */
                std::cout << "[csp::utils::shm::get_shm] shmget succeed! " << std::endl;
            }

            _flag = true;

            return _shmid;
        }


        /**
         * @brief  get shared memory use specified key
         * @param  null
         * @return shm id
         */
        int shm::get_shm(key_t key)
        {
            /* assign to shared memory segment name */
            _shmid = shmget((key_t)key, _shm_size, IPC_CREAT | 0666);
            if(_shmid < 0)
            {
                /* log_output [shmget failed] */
                std::cout << "[csp::utils::shm::get_shm] failed! " << std::endl;
                return -1;
            }
            else
            {
                /* log_output */
                /* shm create succeed */
                std::cout << "[csp::utils::shm::get_shm] shmget succeed! " << std::endl;
            }

            _flag = true;

            return _shmid;
        }


        /**
         * @brief  attach shared memory to the current process
         * @param  null
         * @return void*
         */
        void* shm::attach_shm()
        {
            void* ret = NULL;
            if(_shmid < 0 || !_flag)
            {
                /* log_output [shm isn't create or create failed] */
                std::cout << "[csp::utils::shm::attach_shm] shm create failed! " << std::endl;
                return ret;
            }

            /* get shm address */
            _shm_addr = shmat(_shmid, NULL, 0);
            if(_shm_addr == (void*)-1)
            {
                /* log_output [shmat failed] */
                std::cout << "[csp::utils::shm::attach_shm] shm attach failed! " << std::endl;
                return ret;
            }

            return _shm_addr;
        }


        /**
         * @brief create shm object I/F
         * @param null
         * @return void
         */
        void shm::create_shm()
        {
            get_shm();
            attach_shm();
        }


        /**
         * @brief create shm object I/F
         * @param key_t key
         * @return void
         */
        void shm::create_shm(key_t key)
        {
            get_shm(key);
            attach_shm();
        }


        /**
         * @notice can't write data while reading data
         * @brief read data from shared memory
         * @param null
         * @return void*
         */
        void* shm::read_shm()
        {
            static char shm_read[SHM_BUF_SIZE_DEFAULT] = {0};

            if(!_flag || !_shm_addr)
            {
                /* log_output [shm is not create] */
                std::cout << "[csp::utils::shm::read_shm] shm create failed! " << std::endl;
                return NULL;
            }

            _shm_chunk = (shm_chunk*)_shm_addr;
            _shm_chunk->_rw_flag = 0;

            while(true)
            {
                /* when writing to shm, shm data is not readable */
                /* write to shm done */
                if(_shm_chunk->_rw_flag == 1)
                {
                    /* clear shm buf */
                    memset(shm_read, 0, SHM_BUF_SIZE_DEFAULT);

                    /* after reading the data, make sure shared memory chunk writeable */
                    /* copy shared memory chunk to shm_read buf */
                    memcpy(shm_read, _shm_chunk->_shm_buf, _shm_chunk->_shm_data_len);
                    shm_read[_shm_chunk->_shm_data_len] = '\0';

                    _shm_chunk->_rw_flag = 0;

                    break;
                }
                else
                {
                    /* log_output [sleep 10ms] */
                    csp::utils::sleep_ms(10);
                }
            }

            return shm_read;
        }


        /**
         * @notice writing to shared memory requires lock
         * @brief write data to shared memory
         * @param char* _data
         * @return void
         */
        void shm::write_shm(char* _data)
        {
            if(!_flag || !_shm_addr)
            {
                /* log_output [shm is not create] */
                std::cout << "[csp::utils::shm::write_shm] shm create failed! " << std::endl;
                return;
            }

            /* use mutex lock when writing data */
            _shm_mutex.lock();

            _shm_chunk = (shm_chunk*)_shm_addr;
            memcpy(_shm_chunk->_shm_buf, _data, strlen(_data));
            _shm_chunk->_shm_data_len = strlen(_data);
            _shm_chunk->_rw_flag = 1;  /* data written */

            /* data written and unlock */
            _shm_mutex.unlock();
        }


        /**
         * @brief seperate shared memory from the current process
         * @param null
         * @return 0: OK
         *         -1: NOK
         */
        int shm::detach_shm()
        {
            int ret = -1;
            if(!_shm_addr)
            {
                /* log_output [shm not create] */
                std::cout << "[csp::utils::shm::detach_shm] shm create failed! " << std::endl;
                return ret;
            }

            ret = shmdt(_shm_addr);

            if(-1 == ret)
            {
                /* log_output [shmdt failed] */
                std::cout << "[csp::utils::shm::detach_shm] shm detach failed! " << std::endl;
                return ret;
            }

            return 0;
        }


        /**
         * @brief delete shared memory
         * @param null
         * @return void
         */
        void shm::del_shm()
        {
            if(_shmid < 0)
            {
                /* log_output [shm is not create] */
                std::cout << "[csp::utils::shm::del_shm] shm create failed! " << std::endl;
                return;
            }

            if(shmctl(_shmid, IPC_RMID, 0) == -1)
            {
                /* log_output [shmctl delete failed] */
                std::cout << "[csp::utils::shm::del_shm] shm delete failed! " << std::endl;
                return;
            }

        }


        void shm::set_shm_size(int32_t shm_size)
        {
            _shm_size = shm_size;
        }


        csp_utils_shm_chunk* shm::get_shm_chunk()
        {
            return _shm_chunk;
        }


        int shm::get_shmid()
        {
            return _shmid;
        }


        void* shm::get_shm_addr()
        {
            return _shm_addr;
        }
    }
}

