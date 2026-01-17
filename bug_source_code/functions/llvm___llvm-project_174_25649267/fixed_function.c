void BasicBlock::print(raw_ostream &ROS, AssemblyAnnotationWriter *AAW,
                     bool ShouldPreserveUseListOrder,
                     bool IsForDebug) const {
  SlotTracker SlotTable(this->getParent());
  formatted_raw_ostream OS(ROS);
  AssemblyWriter W(OS, SlotTable, this->getModule(), AAW,
                   IsForDebug,