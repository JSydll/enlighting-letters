/**
 * @file LoginHtml.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef UPDATE__LOGIN_HPP
#define UPDATE__LOGIN_HPP

#include <string>

namespace EnlightingLetters
{

const std::string kUser = "updater";
const std::string kPassword = "ljwedding19";

// clang-format off
const std::string kLoginPage =
    "<form name='loginForm'>"
      "<table width='20%' bgcolor='A09F9F' align='center'>"
        "<tr>"
          "<td colspan=2>"
            "<center><font size=4><b>L&J Login</b></font></center><br>"
          "</td><br><br>"
        "</tr>"
        "<tr>"
          "<td>User:</td>"
          "<td><input type='text' size=25 name='usr'><br></td><br><br>"
        "</tr>"
        "<tr>"
          "<td>Password:</td>"
          "<td><input type='Password' size=25 name='pwd'><br></td><br><br>"
        "</tr>"
        "<tr>"
          "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
        "</tr>"
      "</table>"
    "</form>"
    "<script>"
      "function check(f){ "
        "if(f.usr.value=='" + kUser + "' && form.pwd.value=='" + kPassword + "'){"
          "window.open('/upload')"
        "} else { "
          "alert('Invalid password or username')"
        "}" 
      "}"
    "</script>";
// clang-format on

}  // namespace EnlightingLetters

#endif
