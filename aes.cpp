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

		Pipe pipe(get_cipher("AES-128/CBC", key, iv, ENCRYPTION));

		pipe.process_msg("secrets1234567890ABCabc");

		secure_vector<byte> c1 = pipe.read_all(0);

		cout << "Encrypted:" << endl;
		hexdump(c1.data(), c1.size());
	}
	catch(std::exception& e) {
		cerr << e.what() << endl;
	}
}
