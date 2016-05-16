#include <botan/ecdh.h>
#include <botan/auto_rng.h>
#include <botan/init.h>
#include <botan/ec_group.h>
#include <botan/pubkey.h>
#include <botan/x509_key.h>
#include <iostream>

#include "utils.h"

using namespace Botan;
using namespace std;

int main(){
	try{
		LibraryInitializer init;
		AutoSeeded_RNG rng;

		ECDH_PrivateKey priv1(rng, EC_Group("brainpool512r1"), 0);
		ECDH_PublicKey pub1(priv1.domain(), priv1.public_point());

		ECDH_PrivateKey priv2(rng, EC_Group("brainpool512r1"), 0);
		ECDH_PublicKey pub2(priv2.domain(), priv2.public_point());

		vector<Botan::byte> pub1_vec = X509::BER_encode(pub1);

		ECDH_PublicKey* pub1_trans = dynamic_cast<Botan::ECDH_PublicKey*>
			(X509::load_key(vector<byte>(pub1_vec.data(), pub1_vec.data() + pub1_vec.size())));

		cout << "X509 method size: " << pub1_vec.size() << endl;

		/*
		vector<byte> pub1_trans_vec =  pub1.public_value();
		secure_vector<byte> pub1_trans_vec_sec = secure_vector<byte>(
				pub1_trans_vec.data(), pub1_trans_vec.data() + pub1_trans_vec.size());
		ECDH_PublicKey pub1_trans(priv2.algorithm_identifier(), pub1_trans_vec_sec);
		*/

		PK_Key_Agreement agree1(priv1, "KDF2(SHA-256)", "");
		SymmetricKey key1 = agree1.derive_key(256, pub2.public_value(), "");

		PK_Key_Agreement agree2(priv2, "KDF2(SHA-256)", "");
		SymmetricKey key2 = agree2.derive_key(256, pub1_trans->public_value(), "");

		if(key1.as_string().compare(key2.as_string())){
			cout << "[FAIL] symmetric keys do *not* match" << endl;
		} else {
			cout << "[OKAY] symmetric keys match" << endl;
		}
	}
	catch(std::exception& e){
		cerr << e.what() << endl;
	}
}
