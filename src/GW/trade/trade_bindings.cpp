#include "base/py_bindings.h"

#include "GW/trade/trade.h"

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(PyTrade, m) {
    m.doc() = "Py4GW Trade bindings";

    m.def("open_trade_window", &GW::trade::OpenTradeWindow, py::arg("agent_id"));
    m.def("accept_trade", &GW::trade::AcceptTrade);
    m.def("cancel_trade", &GW::trade::CancelTrade);
    m.def("change_offer", &GW::trade::ChangeOffer);
    m.def("submit_offer", &GW::trade::SubmitOffer, py::arg("gold"));
    m.def("remove_item", &GW::trade::RemoveItem, py::arg("item_id"));
    m.def("offer_item", &GW::trade::OfferItem, py::arg("item_id"), py::arg("quantity") = 0);

    m.def("is_item_offered", [](uint32_t item_id) -> bool {
        return GW::trade::IsItemOffered(item_id) != nullptr;
    }, py::arg("item_id"));
}
