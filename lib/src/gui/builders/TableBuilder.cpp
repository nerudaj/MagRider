#include "gui/builders/TableBuilder.hpp"
#include "gui/Sizers.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include "types/SemanticTypes.hpp"
#include <cassert>
#include <ranges>

void priv::TableBuilder::addRow(const std::vector<tgui::Widget::Ptr>& cells)
{
    assert(!heading || heading->size() == cells.size());
    assert(rowsOfCells.empty() || rowsOfCells.front().size() == cells.size());
    rowsOfCells.push_back(cells);
}

void priv::TableBuilder::addSeparator()
{
    rowsOfCells.push_back({});
}

template<class Range>
NODISCARD_RESULT static auto enumerate(Range&& range)
{
    return std::views::zip(
        std::views::iota(size_t { 0 }, std::ranges::size(range)),
        std::forward<Range>(range));
}

NODISCARD_RESULT static tgui::Widget::Ptr
createCell(const tgui::Widget::Ptr& content, size_t column, size_t totalColumns)
{
    size_t columnWidth = 100 / totalColumns;

    auto cell = WidgetBuilder::createPanel(
        { tgui::Layout(std::to_string(columnWidth) + "%"), "100%" });
    cell->setPosition(
        { tgui::Layout(std::to_string(column * columnWidth) + "%"), "0%" });
    cell->add(content);

    return cell;
}

tgui::Widget::Ptr priv::TableBuilder::build()
{
    auto&& panel = WidgetBuilder::createScrollablePanel();

    assert(heading || !rowsOfCells.empty());

    const auto&& columnCount =
        heading ? heading->size() : rowsOfCells.front().size();

    if (heading)
    {
        auto&& row = WidgetBuilder::createRow(tgui::Color(128, 128, 128, 64));

        for (auto&& [columnIdx, cellText] : enumerate(heading.value()))
        {
            row->add(createCell(
                WidgetBuilder::createTextLabel(cellText, "justify"_true),
                columnIdx,
                columnCount));
        }

        panel->add(row);
    }

    auto&& rowIdx = heading ? 1u : 0u;

    for (auto&& row : rowsOfCells)
    {
        auto&& color = rowIdx % 2 == 0 ? tgui::Color(128, 128, 128, 64)
                                       : tgui::Color::Transparent;
        auto&& rowWidget = WidgetBuilder::createRow(color);
        rowWidget->setPosition({ "0%", rowWidget->getSize().y * rowIdx++ });

        if (row.empty()) // separator
        {
            rowWidget->add(WidgetBuilder::createSeparator());
        }
        else
        {
            for (auto&& [columnIdx, cellText] : enumerate(row))
            {
                rowWidget->add(createCell(cellText, columnIdx, columnCount));
            }
        }

        panel->add(rowWidget);
    }

    return panel;
}
