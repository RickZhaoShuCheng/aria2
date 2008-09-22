/* <!-- copyright */
/*
 * aria2 - The high speed download utility
 *
 * Copyright (C) 2006 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
/* copyright --> */
#ifndef _D_NAME_MATCH_OPTION_HANDLER_H_
#define _D_NAME_MATCH_OPTION_HANDLER_H_

#include "OptionHandler.h"
#include "DlAbortEx.h"
#include "StringFormat.h"
#include "A2STR.h"
#include "Util.h"
#include <strings.h>
#include <algorithm>
#include <sstream>
#include <iterator>

#define NO_DESCRIPTION A2STR::NIL
#define NO_DEFAULT_VALUE A2STR::NIL

namespace aria2 {

class Option;

class NameMatchOptionHandler : public OptionHandler {
protected:
  std::string _optName;

  std::string _description;

  std::string _defaultValue;

  bool _hidden;

  std::deque<std::string> _tags;

  virtual void parseArg(Option* option, const std::string& arg) = 0;
public:
  NameMatchOptionHandler(const std::string& optName,
			 const std::string& description = NO_DESCRIPTION,
			 const std::string& defaultValue = NO_DEFAULT_VALUE,
			 bool hidden = false):
    _optName(optName),
    _description(description),
    _defaultValue(defaultValue),
    _hidden(hidden) {}

  virtual ~NameMatchOptionHandler() {}
  
  virtual bool canHandle(const std::string& optName)
  {
    return strcasecmp(_optName.c_str(), optName.c_str()) == 0;
  }

  virtual void parse(Option* option, const std::string& arg)
  {
    try {
      parseArg(option, arg);
    } catch(Exception& e) {
      throw DlAbortEx
	(StringFormat("Exception occurred while processing option %s",
		      _optName.c_str()).str(), e);
    }
  }

  virtual bool hasTag(const std::string& tag) const
  {
    return std::find(_tags.begin(), _tags.end(), tag) != _tags.end();
  }

  virtual void addTag(const std::string& tag)
  {
    _tags.push_back(tag);
  }

  virtual std::string toTagString() const
  {
    std::stringstream s;
    std::copy(_tags.begin(), _tags.end(),
	      std::ostream_iterator<std::string>(s, ","));
    return Util::trim(s.str(), ", ");
  }

  virtual const std::string& getName() const
  {
    return _optName;
  }

  virtual const std::string& getDescription() const
  {
    return _description;
  }

  virtual const std::string& getDefaultValue() const
  {
    return _defaultValue;
  }

  virtual bool isHidden() const
  {
    return _hidden;
  }

};

typedef SharedHandle<NameMatchOptionHandler> NameMatchOptionHandlerHandle;

} // namespace aria2

#endif // _D_NAME_MATCH_OPTION_HANDLER_H_
