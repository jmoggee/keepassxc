/*
 *  Tokyo Night (Night variant) theme for KeePassXC.
 *  Colours from https://github.com/folke/tokyonight.nvim (night).
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 */

#include "TokyoNightStyle.h"

#include <QDialog>
#include <QFile>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>

namespace
{
    // Tokyo Night "night" palette
    constexpr QRgb BgDark = 0x16161E;
    constexpr QRgb Bg = 0x1A1B26;
    constexpr QRgb BgFloat = 0x1F2335;
    constexpr QRgb BgPanel = 0x24283B;
    constexpr QRgb BgHighlight = 0x292E42;
    constexpr QRgb Gutter = 0x3B4261;
    constexpr QRgb Selection = 0x33467C;
    constexpr QRgb SelectionInactive = 0x283457;
    constexpr QRgb Fg = 0xC0CAF5;
    constexpr QRgb FgDark = 0xA9B1D6;
    constexpr QRgb Dark3 = 0x545C7E;
    constexpr QRgb Comment = 0x565F89;
    constexpr QRgb Blue = 0x7AA2F7;
    constexpr QRgb Magenta = 0xBB9AF7;
} // namespace

QPalette TokyoNightStyle::standardPalette() const
{
    auto palette = BaseStyle::standardPalette();
    palette.setColor(QPalette::Active, QPalette::Window, BgFloat);
    palette.setColor(QPalette::Inactive, QPalette::Window, BgFloat);
    palette.setColor(QPalette::Disabled, QPalette::Window, BgPanel);

    palette.setColor(QPalette::Active, QPalette::WindowText, Fg);
    palette.setColor(QPalette::Inactive, QPalette::WindowText, FgDark);
    palette.setColor(QPalette::Disabled, QPalette::WindowText, Dark3);

    palette.setColor(QPalette::Active, QPalette::Text, Fg);
    palette.setColor(QPalette::Inactive, QPalette::Text, FgDark);
    palette.setColor(QPalette::Disabled, QPalette::Text, Dark3);

    palette.setColor(QPalette::All, QPalette::PlaceholderText, Comment);

    palette.setColor(QPalette::All, QPalette::BrightText, BgDark);

    palette.setColor(QPalette::Active, QPalette::Base, Bg);
    palette.setColor(QPalette::Inactive, QPalette::Base, Bg);
    palette.setColor(QPalette::Disabled, QPalette::Base, BgFloat);

    palette.setColor(QPalette::Active, QPalette::AlternateBase, BgFloat);
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase, BgFloat);
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase, BgPanel);

    palette.setColor(QPalette::All, QPalette::ToolTipBase, BgPanel);
    palette.setColor(QPalette::All, QPalette::ToolTipText, Fg);

    palette.setColor(QPalette::Active, QPalette::Button, BgHighlight);
    palette.setColor(QPalette::Inactive, QPalette::Button, BgHighlight);
    palette.setColor(QPalette::Disabled, QPalette::Button, BgPanel);

    palette.setColor(QPalette::Active, QPalette::ButtonText, Fg);
    palette.setColor(QPalette::Inactive, QPalette::ButtonText, FgDark);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, Dark3);

    palette.setColor(QPalette::Active, QPalette::Highlight, Selection);
    palette.setColor(QPalette::Inactive, QPalette::Highlight, SelectionInactive);
    palette.setColor(QPalette::Disabled, QPalette::Highlight, BgHighlight);

    palette.setColor(QPalette::Active, QPalette::HighlightedText, Fg);
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, Fg);
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, Dark3);

    palette.setColor(QPalette::All, QPalette::Light, BgPanel);
    palette.setColor(QPalette::All, QPalette::Midlight, BgFloat);
    palette.setColor(QPalette::All, QPalette::Mid, Gutter);
    palette.setColor(QPalette::All, QPalette::Dark, BgDark);
    palette.setColor(QPalette::All, QPalette::Shadow, QRgb(0x0F0F14));

    palette.setColor(QPalette::All, QPalette::Link, Blue);
    palette.setColor(QPalette::Disabled, QPalette::Link, Comment);
    palette.setColor(QPalette::All, QPalette::LinkVisited, Magenta);
    palette.setColor(QPalette::Disabled, QPalette::LinkVisited, Comment);

    return palette;
}

QString TokyoNightStyle::getAppStyleSheet() const
{
    QFile extStylesheetFile(QStringLiteral(":/styles/tokyonight/tokyonightstyle.qss"));
    if (extStylesheetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return extStylesheetFile.readAll();
    }
    qWarning("Failed to load Tokyo Night theme stylesheet.");
    return {};
}

void TokyoNightStyle::polish(QWidget* widget)
{
    if (qobject_cast<QMainWindow*>(widget) || qobject_cast<QDialog*>(widget) || qobject_cast<QMenuBar*>(widget)
        || qobject_cast<QToolBar*>(widget)) {
        auto palette = widget->palette();
        palette.setColor(QPalette::Active, QPalette::Window, BgDark);
        palette.setColor(QPalette::Inactive, QPalette::Window, BgDark);
        palette.setColor(QPalette::Disabled, QPalette::Window, BgFloat);
        widget->setPalette(palette);
    }
}
