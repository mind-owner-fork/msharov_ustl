// This file is part of the ustl library, an STL implementation.
// Copyright (C) 2003 by Mike Sharov <msharov@talentg.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the 
// Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
// Boston, MA  02111-1307  USA.
//
// uqueue.h
//

#ifndef UQUEUE_H_27F01FDB0D59B75277E0E5C41ABC6B5B
#define UQUEUE_H_27F01FDB0D59B75277E0E5C41ABC6B5B

namespace ustl {

/// \class queue uqueue.h ustl.h
/// \ingroup Sequences
///
/// \brief Queue adapter to uSTL containers.
///
/// The most efficient way to use this implementation is to fill the queue
/// and the completely empty it before filling again.
///
template <class Sequence>
class queue {
public:
    typedef typename Sequence::value_type	value_type;
    typedef typename Sequence::size_type	size_type;
    typedef typename Sequence::difference_type	difference_type;
    typedef typename Sequence::reference	reference;
    typedef typename Sequence::const_reference	const_reference;
    typedef typename Sequence::pointer		pointer;
public:
				queue (void);
    explicit			queue (const Sequence& s);
    inline bool			empty (void) const;
    inline size_type		size (void) const;
    inline reference		front (void);
    inline const_reference	front (void) const;
    inline reference		back (void);
    inline const_reference	back (void) const;
    inline void			push (const value_type& v);
    inline void			pop (void);
    inline bool			operator== (const queue& s);
    inline bool			operator< (const queue& s);
private:
    Sequence			m_Storage;
    size_t			m_Front;
};

/// Default constructor.
template <class Sequence>
queue<Sequence>::queue (void)
: m_Storage (),
  m_Front (0)
{
}

/// Copies contents of \p s.
template <class Sequence>
queue<Sequence>::queue (const Sequence& s)
: m_Storage (s),
  m_Front (0)
{
}

/// Returns the number of elements.
template <class Sequence>
inline typename queue<Sequence>::size_type queue<Sequence>::size (void) const
{
    return (m_Storage.size() - m_Front);
}

/// Returns true if empty.
template <class Sequence>
inline bool queue<Sequence>::empty (void) const
{
    return (size() == 0);
}

/// Returns the front element.
template <class Sequence>
inline typename queue<Sequence>::reference queue<Sequence>::front (void)
{
    return (m_Storage [m_Front]);
}

/// Returns the front element.
template <class Sequence>
inline typename queue<Sequence>::const_reference queue<Sequence>::front (void) const
{
    return (m_Storage [m_Front]);
}

/// Returns the back element.
template <class Sequence>
inline typename queue<Sequence>::reference queue<Sequence>::back (void)
{
    return (m_Storage.back());
}

/// Returns the back element.
template <class Sequence>
inline typename queue<Sequence>::const_reference queue<Sequence>::back (void) const
{
    return (m_Storage.back());
}

/// Pushes \p v on the queue.
template <class Sequence>
inline void queue<Sequence>::push (const value_type& v)
{
    if (m_Front && m_Storage.size() == m_Storage.capacity()) {
	m_Storage.erase (m_Storage.begin(), m_Front);
	m_Front = 0;
    }
    m_Storage.push_back (v);
}

/// Pops the topmost element from the queue.
template <class Sequence>
inline void queue<Sequence>::pop (void)
{
    if (++m_Front >= m_Storage.size())
	m_Storage.resize (m_Front = 0);
}

/// Compares to \p s.
template <class Sequence>
inline bool queue<Sequence>::operator== (const queue& s)
{
    return (m_Storage == s.m_Storage && m_Front == s.m_Front);
}

/// Compares to \p s.
template <class Sequence>
inline bool queue<Sequence>::operator< (const queue& s)
{
    return (size() < s.size());
}

}; // namespace ustl

#endif
