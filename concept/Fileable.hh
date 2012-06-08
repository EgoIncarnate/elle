#ifndef  ELLE_CONCEPT_FILEABLE_HH
# define ELLE_CONCEPT_FILEABLE_HH

///
/// This module provides the 'Fileable' concept.
/// Requirements:
///   - The class is serializable (XXX make doc on serializable concept)
/// Effects:
///   - Provides two virtual methods: Store(Path) and Load(Path)
///
/// Example 1: This makes 'MyClass' `fileable` with a BinaryArchive
/// ---------------------------------------------------------------------------
///   class MyClass
///     : public elle::concept::Fileable<>
///   {};
///
/// Example 2: When some base class are also fileable
/// ---------------------------------------------------------------------------
///   class MyClass2
///     : public MyClass // MyClass is fileable too
///     , public elle::concept::Fileable<>
///   {
///   public:
///     ELLE_CONCEPT_FILEABLE_METHODS(MyClass2); // desambiguation is needed
///   };
/// ---------------------------------------------------------------------------
///
/// Example 3: If you need an abstract base class that enforce the fileable
///            contract you will need to inherit from AbstractFileable
/// ---------------------------------------------------------------------------
///   class MyClassInterface
///     : public elle::concept::AbstractFileable<>
///   {};
///
///   class MyClassImplem
///     : public elle::concept::Fileable<MyclassImplem>
///   {};
/// ---------------------------------------------------------------------------
///

# include <string>
# include <typeinfo>

# include <elle/concept/Serializable.hh>
# include <elle/io/Path.hh>
# include <elle/serialize/BinaryArchive.fwd.hh>

# define ELLE_CONCEPT_FILEABLE_METHODS(...)                                   \
  using elle::concept::Fileable<__VA_ARGS__>::Store;                          \
  using elle::concept::Fileable<__VA_ARGS__>::Load                            \
  /**/

namespace elle
{
  namespace concept
  {

    template<__ECS_DEFAULT_ARCHIVE_TPL(Archive)>
    struct Fileable
      : virtual contract::_Serializable<Archive>
    {
      Status Load(elle::io::Path const& path);
      Status Store(elle::io::Path const& path) const;
    };

  } // !concept
} // !elle

# include "Fileable.hxx"

#endif
