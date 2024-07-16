#include "Enclave_u.h"
#include <errno.h>
#include "Branch.h"
#include "Node.h"
typedef struct ms_ecall_mbedtls_crypto_t {
	int ms_retval;
} ms_ecall_mbedtls_crypto_t;

typedef struct ms_pathR_t {
	Node* ms_node1;
	Node* ms_node2;
} ms_pathR_t;

typedef struct ms_insert_t {
	Node* ms_node;
} ms_insert_t;

typedef struct ms_erase_t {
	Branch* ms_mBranch;
} ms_erase_t;

typedef struct ms_tempstash_t {
	Branch* ms_mBranch;
} ms_tempstash_t;

typedef struct ms_aes_encrypt_t {
	const char* ms_str;
	char* ms_st;
} ms_aes_encrypt_t;

typedef struct ms_aes_dncrypt_t {
	const char* ms_str;
	char* ms_st;
} ms_aes_dncrypt_t;

typedef struct ms_generate_random_block_t {
	int ms_length;
	char* ms_st;
} ms_generate_random_block_t;

typedef struct ms_ocall_print_string_t {
	const char* ms_str;
} ms_ocall_print_string_t;

typedef struct ms_sgx_oc_cpuidex_t {
	int* ms_cpuinfo;
	int ms_leaf;
	int ms_subleaf;
} ms_sgx_oc_cpuidex_t;

typedef struct ms_sgx_thread_wait_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_self;
} ms_sgx_thread_wait_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_set_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_waiter;
} ms_sgx_thread_set_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_setwait_untrusted_events_ocall_t {
	int ms_retval;
	const void* ms_waiter;
	const void* ms_self;
} ms_sgx_thread_setwait_untrusted_events_ocall_t;

typedef struct ms_sgx_thread_set_multiple_untrusted_events_ocall_t {
	int ms_retval;
	const void** ms_waiters;
	size_t ms_total;
} ms_sgx_thread_set_multiple_untrusted_events_ocall_t;

static sgx_status_t SGX_CDECL Enclave_ocall_print_string(void* pms)
{
	ms_ocall_print_string_t* ms = SGX_CAST(ms_ocall_print_string_t*, pms);
	ocall_print_string(ms->ms_str);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_oc_cpuidex(void* pms)
{
	ms_sgx_oc_cpuidex_t* ms = SGX_CAST(ms_sgx_oc_cpuidex_t*, pms);
	sgx_oc_cpuidex(ms->ms_cpuinfo, ms->ms_leaf, ms->ms_subleaf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_thread_wait_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_wait_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_wait_untrusted_event_ocall(ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_thread_set_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_set_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_untrusted_event_ocall(ms->ms_waiter);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_thread_setwait_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_setwait_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_setwait_untrusted_events_ocall(ms->ms_waiter, ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_thread_set_multiple_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_multiple_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_multiple_untrusted_events_ocall(ms->ms_waiters, ms->ms_total);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[6];
} ocall_table_Enclave = {
	6,
	{
		(void*)Enclave_ocall_print_string,
		(void*)Enclave_sgx_oc_cpuidex,
		(void*)Enclave_sgx_thread_wait_untrusted_event_ocall,
		(void*)Enclave_sgx_thread_set_untrusted_event_ocall,
		(void*)Enclave_sgx_thread_setwait_untrusted_events_ocall,
		(void*)Enclave_sgx_thread_set_multiple_untrusted_events_ocall,
	}
};
sgx_status_t ecall_mbedtls_crypto(sgx_enclave_id_t eid, int* retval)
{
	sgx_status_t status;
	ms_ecall_mbedtls_crypto_t ms;
	status = sgx_ecall(eid, 0, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t pathR(sgx_enclave_id_t eid, Node* node1, Node* node2)
{
	sgx_status_t status;
	ms_pathR_t ms;
	ms.ms_node1 = node1;
	ms.ms_node2 = node2;
	status = sgx_ecall(eid, 1, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t insert(sgx_enclave_id_t eid, Node* node)
{
	sgx_status_t status;
	ms_insert_t ms;
	ms.ms_node = node;
	status = sgx_ecall(eid, 2, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t erase(sgx_enclave_id_t eid, Branch* mBranch)
{
	sgx_status_t status;
	ms_erase_t ms;
	ms.ms_mBranch = mBranch;
	status = sgx_ecall(eid, 3, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t tempstash(sgx_enclave_id_t eid, Branch* mBranch)
{
	sgx_status_t status;
	ms_tempstash_t ms;
	ms.ms_mBranch = mBranch;
	status = sgx_ecall(eid, 4, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t aes_encrypt(sgx_enclave_id_t eid, const char* str, char* st)
{
	sgx_status_t status;
	ms_aes_encrypt_t ms;
	ms.ms_str = str;
	ms.ms_st = st;
	status = sgx_ecall(eid, 5, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t aes_dncrypt(sgx_enclave_id_t eid, const char* str, char* st)
{
	sgx_status_t status;
	ms_aes_dncrypt_t ms;
	ms.ms_str = str;
	ms.ms_st = st;
	status = sgx_ecall(eid, 6, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t generate_random_block(sgx_enclave_id_t eid, int length, char* st)
{
	sgx_status_t status;
	ms_generate_random_block_t ms;
	ms.ms_length = length;
	ms.ms_st = st;
	status = sgx_ecall(eid, 7, &ocall_table_Enclave, &ms);
	return status;
}

