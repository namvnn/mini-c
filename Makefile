TOPTARGETS := run clean

PROJECTS := $(wildcard */.)

.PHONY: $(TOPTARGETS)
$(TOPTARGETS): $(PROJECTS)

.PHONY: $(PROJECTS)
$(PROJECTS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: fmt
fmt:
	@find . -type f \
		\( -name '*.c' -o -name '*.h' \) \
		-not -path '*lib*' \
		-not -path '*.git*' \
		-exec clang-format -i {} '+'
