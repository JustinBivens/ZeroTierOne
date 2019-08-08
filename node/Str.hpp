/*
 * ZeroTier One - Network Virtualization Everywhere
 * Copyright (C) 2011-2019  ZeroTier, Inc.  https://www.zerotier.com/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * --
 *
 * You can be released from the requirements of the license by purchasing
 * a commercial license. Buying such a license is mandatory as soon as you
 * develop commercial closed-source software that incorporates or links
 * directly against ZeroTier software without disclosing the source code
 * of your own application.
 */

#ifndef ZT_STR_HPP
#define ZT_STR_HPP

#include "Constants.hpp"
#include "Utils.hpp"
#include "Address.hpp"
#include "MAC.hpp"
#include "InetAddress.hpp"

#define ZT_STR_CAPACITY 254

namespace ZeroTier {

class Str
{
public:
	Str() { _l = 0; _s[0] = 0; }
	Str(const Str &s)
	{
		_l = s._l;
		memcpy(_s,s._s,_l+1);
	}
	Str(const char *s)
	{
		_l = 0;
		_s[0] = 0;
		(*this) << s;
	}

	inline Str &operator=(const Str &s)
	{
		_l = s._l;
		memcpy(_s,s._s,_l+1);
		return *this;
	}
	inline Str &operator=(const char *s)
	{
		_l = 0;
		_s[0] = 0;
		return ((*this) << s);
	}

	inline char operator[](const unsigned int i) const
	{
		if (unlikely(i >= (unsigned int)_l))
			throw ZT_EXCEPTION_OUT_OF_BOUNDS;
		return _s[i];
	}

	inline void clear() { _l = 0; _s[0] = 0; }
	inline const char *c_str() const { return _s; }
	inline unsigned int length() const { return _l; }

	inline Str &operator<<(const char *s)
	{
		if (likely(s != (const char *)0)) {
			unsigned long l = _l;
			while (*s) {
				if (unlikely(l >= ZT_STR_CAPACITY)) {
					_s[l] = 0;
					_l = (uint8_t)l;
					throw ZT_EXCEPTION_OUT_OF_BOUNDS;
				}
				_s[l++] = *s;
			}
			_s[l] = 0;
			_l = (uint8_t)l;
		}
		return *this;
	}
	inline Str &operator<<(const Str &s) { return ((*this) << s._s); }
	inline Str &operator<<(const char c)
	{
		if (likely(c != 0)) {
			if (unlikely(_l >= ZT_STR_CAPACITY)) {
				_s[_l] = 0;
				throw ZT_EXCEPTION_OUT_OF_BOUNDS;
			}
			_s[_l++] = c;
			_s[_l] = 0;
		}
	}
	inline Str &operator<<(const unsigned long n)
	{
		char tmp[32];
		Utils::decimal(n,tmp);
		return ((*this) << tmp);
	}
	inline Str &operator<<(const unsigned int n)
	{
		char tmp[32];
		Utils::decimal((unsigned long)n,tmp);
		return ((*this) << tmp);
	}
	inline Str &operator<<(const Address &a)
	{
		char tmp[32];
		return ((*this) << a.toString(tmp));
	}
	inline Str &operator<<(const InetAddress &a)
	{
		char tmp[128];
		return ((*this) << a.toString(tmp));
	}
	inline Str &operator<<(const MAC &a)
	{
		char tmp[64];
		return ((*this) << a.toString(tmp));
	}

	inline bool operator==(const Str &s) const { return ((_l == s._l)&&(strcmp(_s,s._s) == 0)); }
	inline bool operator!=(const Str &s) const { return ((_l != s._l)||(strcmp(_s,s._s) != 0)); }
	inline bool operator<(const Str &s) const { return ((_l < s._l)&&(strcmp(_s,s._s) < 0)); }
	inline bool operator>(const Str &s) const { return ((_l > s._l)&&(strcmp(_s,s._s) > 0)); }
	inline bool operator<=(const Str &s) const { return ((_l <= s._l)&&(strcmp(_s,s._s) <= 0)); }
	inline bool operator>=(const Str &s) const { return ((_l >= s._l)&&(strcmp(_s,s._s) >= 0)); }

	inline bool operator==(const char *s) const { return (strcmp(_s,s) == 0); }
	inline bool operator!=(const char *s) const { return (strcmp(_s,s) != 0); }
	inline bool operator<(const char *s) const { return (strcmp(_s,s) < 0); }
	inline bool operator>(const char *s) const { return (strcmp(_s,s) > 0); }
	inline bool operator<=(const char *s) const { return (strcmp(_s,s) <= 0); }
	inline bool operator>=(const char *s) const { return (strcmp(_s,s) >= 0); }

private:
	uint8_t _l;
	char _s[ZT_STR_CAPACITY+1];
};

} // namespace ZeroTier

#endif
