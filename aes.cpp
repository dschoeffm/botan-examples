#include <botan/init.h>
#include <botan/auto_rng.h>
#include <botan/symkey.h>
#include <botan/pipe.h>
#include <botan/cipher_mode.h>
#include <botan/key_filt.h>
#include <iostream>
#include "utils.h"

using namespace Botan;
using namespace std;

int main() {
	try {
		LibraryInitializer init;

		AutoSeeded_RNG rng;
		if(!rng.is_seeded()){
			cerr << "RNG was not seeded" << endl;
		}

		SymmetricKey key(rng, 16);
		InitializationVector iv(rng, 16);

		cout << "Key:" << endl;
		hexdump(key.bits_of().data(), 16);
		cout << "IV:" << endl;
		hexdump(iv.bits_of().data(), 16);

		// Encrypt
		Pipe enc(get_cipher("AES-128/CBC", key, iv, ENCRYPTION));
		enc.process_msg("secrets1234567890ABCabc");
		secure_vector<byte> enc_vec = enc.read_all(0);

		cout << "Encrypted:" << endl;
		hexdump(enc_vec.data(), enc_vec.size());

		// Decrypt
		Pipe dec(get_cipher("AES-128/CBC", key, iv, DECRYPTION));
		dec.process_msg(enc_vec);
		secure_vector<byte> dec_vec = dec.read_all(0);

		cout << "Decrypted:" << endl;
		hexdump(dec_vec.data(), dec_vec.size());
		string dec_str((const char*) dec_vec.data(), dec_vec.size());
		cout << "Decrypted String: " << dec_str << endl;
	}
	catch(std::exception& e) {
		cerr << e.what() << endl;
	}
}
