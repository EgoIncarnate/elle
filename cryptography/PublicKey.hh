//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// author        julien quintard   [tue oct 30 01:08:16 2007]
//

#ifndef ELLE_CRYPTOGRAPHY_PUBLICKEY_HH
#define ELLE_CRYPTOGRAPHY_PUBLICKEY_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/types.hh>

#include <elle/radix/Object.hh>

#include <elle/serialize/Uniquable.hh>

#include <elle/cryptography/Plain.hh>
#include <elle/cryptography/Code.hh>
#include <elle/cryptography/Clear.hh>
#include <elle/cryptography/Signature.hh>

#include <elle/idiom/Close.hh>
# include <openssl/rsa.h>
# include <openssl/bn.h>
# include <openssl/err.h>
# include <openssl/evp.h>
#include <elle/idiom/Open.hh>

namespace elle
{

  using namespace radix;

  namespace cryptography
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this class represents a public key.
    ///
    class PublicKey:
        public Object
      , public elle::serialize::Uniquable<PublicKey>
    {
    public:
      //
      // constants
      //
      static const PublicKey            Null;

      //
      // constructors & destructors
      //
      PublicKey();
      PublicKey(const PublicKey&);
      ~PublicKey();

      //
      // methods
      //
      Status            Create(const ::EVP_PKEY*);
      Status            Create(Large*,
                               Large*);

      Status            Encrypt(const Plain&,
                                Code&) const;
      Status            Verify(const Signature&,
                               const Plain&) const;
      Status            Decrypt(const Code&,
                                Clear&) const;
      //
      // interfaces
      //

      // object
      declare(PublicKey);
      Boolean           operator==(const PublicKey&) const;

      // dumpable
      Status            Dump(const Natural32 = 0) const;

      // archivable
      // XXX
      //Status            Serialize(Archive&) const;
      //Status            Extract(Archive&);

      //
      // attributes
      //
    private:
      ::EVP_PKEY*       _key;

      struct
      {
        ::EVP_PKEY_CTX* encrypt;
        ::EVP_PKEY_CTX* verify;
        ::EVP_PKEY_CTX* decrypt;
      }                 _contexts;

    public:
      //
      // properties
      //
      ::EVP_PKEY const* key() const { return this->_key; }

    public:
      ///
      /// this methods are required because the compiler, given an Archive
      /// object will call a template-based method instead of the Plain one.
      ///
      /// we do not want this especially because the template-based methods
      /// build archives and we are already receiving an archive.
      ///

      // XXX
      //Status            Encrypt(const Archive&          archive,
      //                          Code&                   code) const
      //{
      //  return (this->Encrypt(Plain(archive.contents, archive.size),
      //                        code));
      //}

      //Status            Verify(const Signature&         signature,
      //                         const Archive&           archive) const
      //{
      //  return (this->Verify(signature,
      //                       Plain(archive.contents, archive.size)));
      //}

      //Status            Decrypt(const Code&             code,
      //                          Archive&                archive) const
      //{
      //  Clear           clear;

      //  // decrypt the code.
      //  if (this->Decrypt(code, clear) == Status::Error)
      //    escape("unable to decrypt the code");

      //  // prepare the archive.
      //  if (archive.Acquire(clear) == Status::Error)
      //    escape("unable to prepare the archive");

      //  // detach the data so that not both the clear and archive
      //  // release the data.
      //  if (clear.Detach() == Status::Error)
      //    escape("unable to detach the clear's data");

      //  return Status::Ok;
      //}

      //
      // variadic templates
      //

      // encrypt
      template <typename T1>
      Status            Encrypt(const T1&,
                                Code&) const;
      template <typename T1,
                typename T2>
      Status            Encrypt(const T1&,
                                const T2&,
                                Code&) const;
      template <typename T1,
                typename T2,
                typename T3>
      Status            Encrypt(const T1&,
                                const T2&,
                                const T3&,
                                Code&) const;
      template <typename T1,
                typename T2,
                typename T3,
                typename T4>
      Status            Encrypt(const T1&,
                                const T2&,
                                const T3&,
                                const T4&,
                                Code&) const;
      template <typename T1,
                typename T2,
                typename T3,
                typename T4,
                typename T5>
      Status            Encrypt(const T1&,
                                const T2&,
                                const T3&,
                                const T4&,
                                const T5&,
                                Code&) const;
      template <typename T1,
                typename T2,
                typename T3,
                typename T4,
                typename T5,
                typename T6>
      Status            Encrypt(const T1&,
                                const T2&,
                                const T3&,
                                const T4&,
                                const T5&,
                                const T6&,
                                Code&) const;
      template <typename T1,
                typename T2,
                typename T3,
                typename T4,
                typename T5,
                typename T6,
                typename T7>
      Status            Encrypt(const T1&,
                                const T2&,
                                const T3&,
                                const T4&,
                                const T5&,
                                const T6&,
                                const T7&,
                                Code&) const;
      template <typename T1,
                typename T2,
                typename T3,
                typename T4,
                typename T5,
                typename T6,
                typename T7,
                typename T8>
      Status            Encrypt(const T1&,
                                const T2&,
                                const T3&,
                                const T4&,
                                const T5&,
                                const T6&,
                                const T7&,
                                const T8&,
                                Code&) const;
      template <typename T1,
                typename T2,
                typename T3,
                typename T4,
                typename T5,
                typename T6,
                typename T7,
                typename T8,
                typename T9>
      Status            Encrypt(const T1&,
                                const T2&,
                                const T3&,
                                const T4&,
                                const T5&,
                                const T6&,
                                const T7&,
                                const T8&,
                                const T9&,
                                Code&) const;

      // verify
      template <typename T,
                typename... TT>
      Status            Verify(const Signature&,
                               const T&,
                               const TT&...) const;

      // decrypt
      template <typename T,
                typename... TT>
      Status            Decrypt(const Code&,
                                T&,
                                TT&...) const;
    };

  }
}

//
// ---------- templates -------------------------------------------------------
//

#include <elle/cryptography/PublicKey.hxx>

#endif
