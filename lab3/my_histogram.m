filename = 'histogram.txt';
values = importdata(filename);
filename = 'settings.txt';
settings = importdata(filename);
t = num2cell(settings);
[interval, intervals_nr, samples_nr, samples_avg] = deal(t{:});
set(gcf,'units','normalized','outerposition',[0 0 1 1]); %maximize plot
values = values';
figure1 = bar(values);
title('Histogram');
xlabel('Intervals'); % x-axis label
ylabel('Quantity'); % y-axis label
dim = [.725 .68 .2 .2];
str = {'Interval used = '+ string(interval)
    'Nr. of intervals = '+ string(intervals_nr)
    'Samples used = '+ string(samples_nr)
    'Samples avg. = '+ string(samples_avg)
    '0 is between position 9 and 10'};
annotation('textbox',dim,'String',str,'FitBoxToText','on');
saveas(figure1,'picture.png')
