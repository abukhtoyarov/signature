#include "md5.h"

#include <cryptopp/config_int.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>

using namespace std;
using namespace CryptoPP;
using namespace CryptoPP::Weak;

namespace sig {

string calcHash(const char* data, size_t sz) {
  MD5 hash;
  hash.Update(reinterpret_cast<const CryptoPP::byte*>(data), sz);

  string digest;
  digest.resize(hash.DigestSize());
  hash.Final(reinterpret_cast<CryptoPP::byte*>(digest.data()));

  string encoded;
  HexEncoder encoder;
  // encoder will free sink, we shouldn't worry about owning sink instance
  encoder.Attach(new CryptoPP::StringSink(encoded));
  encoder.Put(reinterpret_cast<CryptoPP::byte*>(digest.data()), digest.size());
  encoder.MessageEnd();

  return encoded;
}

string calcHash(const string& data) {
  return calcHash(data.data(), data.size());
}

}  // namespace sig
