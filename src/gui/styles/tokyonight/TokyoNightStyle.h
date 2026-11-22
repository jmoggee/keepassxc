/*
 *  Tokyo Night (Night variant) theme for KeePassXC.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 */

#ifndef KEEPASSXC_TOKYONIGHTSTYLE_H
#define KEEPASSXC_TOKYONIGHTSTYLE_H

#include "gui/styles/base/BaseStyle.h"

class TokyoNightStyle : public BaseStyle
{
    Q_OBJECT

public:
    TokyoNightStyle() = default;
    QPalette standardPalette() const override;

    using BaseStyle::polish;
    void polish(QWidget* widget) override;

protected:
    QString getAppStyleSheet() const override;
};

#endif // KEEPASSXC_TOKYONIGHTSTYLE_H
