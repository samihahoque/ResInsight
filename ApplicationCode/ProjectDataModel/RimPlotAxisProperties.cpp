/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-2018 Statoil ASA
//  Copyright (C) 2019-     Equinor ASA
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "RimPlotAxisProperties.h"

#include "RiaDefines.h"

#include "RimRiuQwtPlotOwnerInterface.h"

#include "cafPdmUiSliderEditor.h"

#include <cmath>

// clang-format off
namespace caf
{
template<>
void caf::AppEnum<RimPlotAxisProperties::NumberFormatType>::setUp()
{
    addItem(RimPlotAxisProperties::NUMBER_FORMAT_AUTO,       "NUMBER_FORMAT_AUTO",       "Auto");
    addItem(RimPlotAxisProperties::NUMBER_FORMAT_DECIMAL,    "NUMBER_FORMAT_DECIMAL",    "Decimal");
    addItem(RimPlotAxisProperties::NUMBER_FORMAT_SCIENTIFIC, "NUMBER_FORMAT_SCIENTIFIC", "Scientific");

    setDefault(RimPlotAxisProperties::NUMBER_FORMAT_AUTO);
}

template<>
void caf::AppEnum<RimPlotAxisProperties::AxisTitlePositionType>::setUp()
{
    addItem(RimPlotAxisProperties::AXIS_TITLE_CENTER, "AXIS_TITLE_CENTER",   "Center");
    addItem(RimPlotAxisProperties::AXIS_TITLE_END,    "AXIS_TITLE_END",      "At End");

    setDefault(RimPlotAxisProperties::AXIS_TITLE_CENTER);
}
} // namespace caf

CAF_PDM_SOURCE_INIT(RimPlotAxisProperties, "SummaryYAxisProperties");

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimPlotAxisProperties::RimPlotAxisProperties()
    : m_enableTitleTextSettings(true)
{
    CAF_PDM_InitObject("Axis Properties", ":/LeftAxis16x16.png", "", "");

    CAF_PDM_InitField(&m_isActive, "Active", true, "Active", "", "", "");
    m_isActive.uiCapability()->setUiHidden(true);

    CAF_PDM_InitFieldNoDefault(&m_name, "Name", "Name", "", "", "");
    m_name.uiCapability()->setUiHidden(true);

    CAF_PDM_InitField(&isAutoTitle, "AutoTitle", true, "Auto Title", "", "", "");
    
    CAF_PDM_InitField(&m_displayLongName,   "DisplayLongName",  true,   "   Names", "", "", "");
    CAF_PDM_InitField(&m_displayShortName,  "DisplayShortName", false,  "   Acronyms", "", "", "");
    CAF_PDM_InitField(&m_displayUnitText,   "DisplayUnitText",  true,   "   Units", "", "", "");

    CAF_PDM_InitFieldNoDefault(&customTitle,        "CustomTitle",      "Title", "", "", "");
    CAF_PDM_InitFieldNoDefault(&titlePositionEnum,  "TitlePosition",    "Title Position", "", "", "");
    CAF_PDM_InitField(&titleFontSize,               "FontSize", 11,     "Font Size", "", "", "");

    CAF_PDM_InitField(&visibleRangeMax, "VisibleRangeMax", RiaDefines::maximumDefaultValuePlot(), "Max", "", "", "");
    CAF_PDM_InitField(&visibleRangeMin, "VisibleRangeMin", RiaDefines::minimumDefaultValuePlot(), "Min", "", "", "");

    CAF_PDM_InitFieldNoDefault(&numberFormat,   "NumberFormat",         "Number Format", "", "", "");
    CAF_PDM_InitField(&numberOfDecimals,        "Decimals", 2,          "Number of Decimals", "", "", "");
    CAF_PDM_InitField(&scaleFactor,             "ScaleFactor", 1.0,     "Scale Factor", "", "", "");
    CAF_PDM_InitField(&valuesFontSize,          "ValuesFontSize", 11,   "Font Size", "", "", "");

    numberOfDecimals.uiCapability()->setUiEditorTypeName(caf::PdmUiSliderEditor::uiEditorTypeName());

    CAF_PDM_InitField(&m_isAutoZoom, "AutoZoom", true, "Set Range Automatically", "", "", "");
    CAF_PDM_InitField(&isLogarithmicScaleEnabled, "LogarithmicScale", false, "Logarithmic Scale", "", "", "");

    updateOptionSensitivity();
}
// clang-format on

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimPlotAxisProperties::setEnableTitleTextSettings(bool enable)
{
    m_enableTitleTextSettings = enable;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
caf::PdmFieldHandle* RimPlotAxisProperties::userDescriptionField()
{
    return &m_name;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QList<caf::PdmOptionItemInfo> RimPlotAxisProperties::calculateValueOptions(const caf::PdmFieldHandle* fieldNeedingOptions,
                                                                              bool*                      useOptionsOnly)
{
    QList<caf::PdmOptionItemInfo> options;
    *useOptionsOnly = true;

    if (&titleFontSize == fieldNeedingOptions || &valuesFontSize == fieldNeedingOptions)
    {
        std::vector<int> fontSizes;
        fontSizes.push_back(8);
        fontSizes.push_back(9);
        fontSizes.push_back(10);
        fontSizes.push_back(11);
        fontSizes.push_back(12);
        fontSizes.push_back(14);
        fontSizes.push_back(16);
        fontSizes.push_back(18);
        fontSizes.push_back(24);

        for (int value : fontSizes)
        {
            QString text = QString("%1").arg(value);
            options.push_back(caf::PdmOptionItemInfo(text, value));
        }
    }
    else if (fieldNeedingOptions == &scaleFactor)
    {
        for (int exp = -12; exp <= 12; exp += 3)
        {
            QString uiText = exp == 0 ? "1" : QString("10 ^ %1").arg(exp);
            double  value  = std::pow(10, exp);

            options.push_back(caf::PdmOptionItemInfo(uiText, value));
        }
    }

    return options;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimPlotAxisProperties::defineUiOrdering(QString uiConfigName, caf::PdmUiOrdering& uiOrdering)
{
    if (m_enableTitleTextSettings)
    {
        caf::PdmUiGroup* titleTextGroup = uiOrdering.addNewGroup("Title Text");

        titleTextGroup->add(&isAutoTitle);

        if (isAutoTitle())
        {
            titleTextGroup->add(&m_displayLongName);
            titleTextGroup->add(&m_displayShortName);
            titleTextGroup->add(&m_displayUnitText);

            customTitle.uiCapability()->setUiReadOnly(true);
        }
        else
        {
            titleTextGroup->add(&customTitle);
            customTitle.uiCapability()->setUiReadOnly(false);
        }
    }

    {
        caf::PdmUiGroup* titleGroup = uiOrdering.addNewGroup("Title Layout");
        titleGroup->add(&titlePositionEnum);
        titleGroup->add(&titleFontSize);
    }

    caf::PdmUiGroup& scaleGroup = *(uiOrdering.addNewGroup("Axis Values"));
    scaleGroup.add(&isLogarithmicScaleEnabled);
    scaleGroup.add(&numberFormat);

    if (numberFormat() != NUMBER_FORMAT_AUTO)
    {
        scaleGroup.add(&numberOfDecimals);
        scaleGroup.add(&scaleFactor);
    }

    scaleGroup.add(&visibleRangeMin);
    scaleGroup.add(&visibleRangeMax);
    scaleGroup.add(&valuesFontSize);

    uiOrdering.skipRemainingFields(true);
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimPlotAxisProperties::setNameAndAxis(const QString& name, QwtPlot::Axis axis)
{
    m_name = name;
    m_axis = axis;

    if (axis == QwtPlot::yRight) this->setUiIcon(QIcon(":/RightAxis16x16.png"));
    if (axis == QwtPlot::xBottom) this->setUiIcon(QIcon(":/BottomAxis16x16.png"));
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QwtPlot::Axis RimPlotAxisProperties::qwtPlotAxisType() const
{
    return m_axis;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RiaDefines::PlotAxis RimPlotAxisProperties::plotAxisType() const
{
    if (m_axis == QwtPlot::yRight) return RiaDefines::PLOT_AXIS_RIGHT;
    if (m_axis == QwtPlot::xBottom) return RiaDefines::PLOT_AXIS_BOTTOM;

    return RiaDefines::PLOT_AXIS_LEFT;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RimPlotAxisProperties::useAutoTitle() const
{
    return isAutoTitle();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RimPlotAxisProperties::showDescription() const
{
    return m_displayLongName();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RimPlotAxisProperties::showAcronym() const
{
    return m_displayShortName();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RimPlotAxisProperties::showUnitText() const
{
    return m_displayUnitText();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RimPlotAxisProperties::isAutoZoom() const
{
    return m_isAutoZoom();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimPlotAxisProperties::setAutoZoom(bool enableAutoZoom)
{
    m_isAutoZoom = enableAutoZoom;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RimPlotAxisProperties::isActive() const
{
    return m_isActive;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimPlotAxisProperties::fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue,
                                                const QVariant& newValue)
{
    if (changedField == &isAutoTitle)
    {
        updateOptionSensitivity();
    }
    else if (changedField == &visibleRangeMax)
    {
        if (visibleRangeMin > visibleRangeMax) visibleRangeMax = oldValue.toDouble();

        m_isAutoZoom = false;
    }
    else if (changedField == &visibleRangeMin)
    {
        if (visibleRangeMin > visibleRangeMax) visibleRangeMin = oldValue.toDouble();

        m_isAutoZoom = false;
    }

    RimRiuQwtPlotOwnerInterface* parentPlot = nullptr;
    this->firstAncestorOrThisOfType(parentPlot);
    if (parentPlot)
    {
        if (changedField == &isLogarithmicScaleEnabled)
        {
            parentPlot->updateAxisScaling();
        }
        else
        {
            parentPlot->updateAxisDisplay();
        }
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimPlotAxisProperties::updateOptionSensitivity()
{
    customTitle.uiCapability()->setUiReadOnly(isAutoTitle);
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimPlotAxisProperties::initAfterRead()
{
    updateOptionSensitivity();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
caf::PdmFieldHandle* RimPlotAxisProperties::objectToggleField()
{
    return &m_isActive;
}