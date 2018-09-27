%.gif: %.pdf
	convert -density 250 -alpha remove -background white $^ $@
