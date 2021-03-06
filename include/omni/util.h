/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 * 
 * This file is part of Omnidome.
 * 
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OMNI_UTIL_H_
#define OMNI_UTIL_H_

#include <memory>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QString>
#include <omni/exception.h>

namespace omni
{
  namespace util
  {
    /// Pointer deleter functor
    struct QtDeleter
    {
      template<typename QOBJECT>
      void operator()(QOBJECT* _obj)
      {
        if (!_obj->parent())
          delete _obj;
      }
    };

    /// QUniquePtr for QObjects
    template<typename T>
    using QUniquePtr = std::unique_ptr<T,QtDeleter>;
  
    /// Linear interpolation between two values
    template<typename T, typename A>
    T mix(const T& _x, const T& _y, A _a)
    {
      return _x*(1.0-_a) + _y * _a;
    } 

    /// Return content of file from a file name
    inline static QString fileToStr(const QString& _filename)
    {
      QFile _f(_filename);
      _f.open(QIODevice::ReadOnly|QIODevice::Text);
      return _f.readAll();
    }

    template<typename STRING>
    STRING removeFileExt(STRING const& _filename)
    {
      int _lastIndex = _filename.lastIndexOf(".");
      if (_lastIndex == -1) return _filename;

      QString _rawName;
      for (int i = 0; i < _lastIndex; ++i)
        _rawName.push_back(_filename[i]);
      return _rawName;
    }


    /// Test if two instances (which inherit from SerializationInterface) have equal data
    template<typename T>
    bool testPtrEqual(T const* _a, T const* _b)
    {
      return _a && _b ? 
        // Call equal() member function from SerializationInterface
        _a->equal(_b) : // Otherwise 
        // Compare pointer adresses 
        (_a == _b);
    }

    /// Test if two vectors which hold unique_ptr's of SerializationInterfaces are equal
    template<typename T, typename F>
    bool testPtrVectorEqual(T const& _a, T const& _b, F f)
    { 
      // Size of vector must be equal
      if (_a.size() != _b.size()) return false;

      // Test each element for equality
      size_t _size = _a.size();
      for (size_t i = 0; i < _size; ++i)
      {
        // Call functor with pointers as arguments
        if (!f(_a[i],_b[i])) return false;
      }
      return true;
    }

    /// Test if two vectors are equal  
    template<typename T>
    bool testPtrVectorEqual(T const& _a, T const& _b)
    {
      typedef typename T::value_type value_type;
      typedef typename value_type::element_type element_type;
      return testPtrVectorEqual(_a,_b,testPtrEqual<element_type>);
    }


    /// Serialize object behind SerializationInterface to stream
    template<typename STREAM, typename T>
    void serializePtr(STREAM& _stream, T const* _t)
    {
      if (!_t)
      {
        _stream << QString("none");
        return;
      }

      _stream << _t->getTypeId();
      _t->toStream(_stream);
    }

    /**@brief Deserialize a pointer from stream.
       @detail Functor f must return a pointer which is constructed 
               from a factory with an id.
               If pointer is not null, it will be deserialized from stream.
               Otherwise, a serialization exception is thrown.
     **/
    template<typename STREAM, typename F>
    void deserializePtr(STREAM& _stream, F f)
    {
      omni::Id _id;
      _stream >> _id;
      if (_id == "none") return;

      auto* _ptr = f(_id);
      if (_ptr) 
      {
        _ptr->fromStream(_stream);
      }
      else
      {
        throw omni::exception::Serialization(_id);
      }
    }
  }

  using util::mix;
  using util::QUniquePtr;
}

#define OMNI_TEST_MEMBER_EQUAL(member)\
  (_lhs.member == _rhs.member)

#define OMNI_TEST_PTR_MEMBER_EQUAL(member)\
  omni::util::testPtrEqual(_lhs.member.get(),_rhs.member.get()) 

#endif /* OMNI_UTIL_H_ */
